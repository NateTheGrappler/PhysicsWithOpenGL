#pragma once
#include "Scene.h"
#include "cmath"

class BlackHole2D_Scene : public Scene
{
public:
    struct blackHole2D
    {
        double r_s; //the event horizon (Schwarzschild radius)
        double mass;
        glm::vec3 position;
        glm::vec3 color;
        double renderScale;
        std::string name = "SagA";
        std::string texture = "BlackHoleSide";

        //function to init struct
        blackHole2D(double mass, glm::vec3 position, glm::vec3 color, double pixelRadius)
            :mass(mass), position(position), color(color)
        {
            calculateR_S(299792458, 6.67 * std::pow(10, -11));
            calculateRenderScale(pixelRadius);
        }


        void calculateRenderScale(double pixelRadius) { renderScale = pixelRadius / r_s; }
        float getRenderRadius() const { return static_cast<float>(r_s * renderScale); }
        void calculateR_S(double speedofLight, double G) { r_s = (2 * G * mass) / (speedofLight * speedofLight); }
    };
    struct rayState
    {
        float r;    //radius value away from bh
        float phi;  //angle in polar coords
        float dr;   //change in radius from bh over time
        float dphi; //change in the angle over time
    };

    //light rays that follow polar coordinates when moving around the black hole (only work for one whole at a time)
    struct lightRayPolar
    {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec2 direction;

        float r;    //radius value away from bh
        float phi;  //angle in polar coords
        float dr;   //change in radius from bh over time
        float dphi; //change in the angle over time

        std::vector<glm::vec2> trail;
        bool continueStep = true;
        bool m_initialized = false;

        lightRayPolar(glm::vec3 pos, glm::vec3 color, glm::vec2 dir)
            :position(pos), color(color), direction(dir)
        {
        }

        rayState calculateDerivatives(const rayState& state, float r_s)
        {
            rayState derivedState;

            //just update regular vals with their derivatives
            derivedState.r = state.dr;  // dr/dλ = dr
            derivedState.phi = state.dphi; // dφ/dλ = dphi

            //avoid division by zero
            if (state.r < 0.01f) {
                derivedState.dr = 0;
                derivedState.dphi = 0;
                return derivedState;
            }


            //derive the second derivative and then update the derivatives with that

            double one_minus_rs_r = 1.0 - r_s / state.r;
            double newton_term = -(r_s / (2.0f * state.r * state.r));
            double relativistic_term = (state.r - 1.5f * r_s) * state.dphi * state.dphi;


            //d²r/dλ² = - (r_s/(2r²))(dr/dλ)²/(1 - r_s/r) + (r - 3r_s/2)(dφ/dλ)²
            derivedState.dr = newton_term + relativistic_term;

            //d²φ/dλ² = -2(dr/dλ)(dφ/dλ)/r + (r_s/(r²(1 - r_s/r)))(dr/dλ)(dφ/dλ)
            derivedState.dphi = -2.0f * state.dr * state.dphi / state.r
                + (r_s * state.dr * state.dphi) / (state.r * state.r * one_minus_rs_r);


            return derivedState;

        }
        rayState rk4Step(const rayState& state, float r_s, float deltaTime)
        {
            //take the derivative of the current values for the ray, and then step once with them
            rayState k1 = calculateDerivatives(state, r_s);
            rayState s2 = { state.r + k1.r * deltaTime / 2,
                           state.phi + k1.phi * deltaTime / 2,
                           state.dr + k1.dr * deltaTime / 2,
                           state.dphi + k1.dphi * deltaTime / 2 };

            //do the same, but now one step forward
            rayState k2 = calculateDerivatives(s2, r_s);
            rayState s3 = { state.r + k2.r * deltaTime / 2,
                           state.phi + k2.phi * deltaTime / 2,
                           state.dr + k2.dr * deltaTime / 2,
                           state.dphi + k2.dphi * deltaTime / 2 };

            //third step calculation given second step as input
            rayState k3 = calculateDerivatives(s3, r_s);
            rayState s4 = { state.r + k3.r * deltaTime,
                           state.phi + k3.phi * deltaTime,
                           state.dr + k3.dr * deltaTime,
                           state.dphi + k3.dphi * deltaTime };

            rayState k4 = calculateDerivatives(s4, r_s);

            //average all of the steps together to get the best approximation for movement
            //with more weight given to the first two rays calculations
            rayState next;
            next.r = state.r + (deltaTime / 6.0f) * (k1.r + 2 * k2.r + 2 * k3.r + k4.r);
            next.phi = state.phi + (deltaTime / 6.0f) * (k1.phi + 2 * k2.phi + 2 * k3.phi + k4.phi);
            next.dr = state.dr + (deltaTime / 6.0f) * (k1.dr + 2 * k2.dr + 2 * k3.dr + k4.dr);
            next.dphi = state.dphi + (deltaTime / 6.0f) * (k1.dphi + 2 * k2.dphi + 2 * k3.dphi + k4.dphi);

            return next;
        }
        void step(float deltaTime, const std::vector<blackHole2D>& blackHoles, float speed)
        {
            for (auto& bh : blackHoles)
            {
                //get the cartesian coords for the distance between the blackhole and the head of the ray
                float dx = position.x - bh.position.x;
                float dy = position.y - bh.position.y;

                if (!m_initialized)
                {
                    //intialize the rays polar values
                    r = std::sqrt(dx * dx + dy * dy);
                    phi = std::atan2(dy, dx);


                    glm::vec2 radial = glm::normalize(glm::vec2(dx, dy));
                    glm::vec2 tangential = glm::normalize(glm::vec2(-radial.y, radial.x));

                    //apply calculations to the velocity values
                    dr = glm::dot(direction, radial) * speed;
                    dphi = glm::dot(direction, tangential) * speed / r;

                    m_initialized = true;
                }

                //if collided with blackhole, quit moving
                if (r <= bh.getRenderRadius()) { continueStep = false; }

                //based on the current state, interpolate 4 forward steps
                rayState current = { r, phi, dr, dphi };
                rayState next = rk4Step(current, bh.getRenderRadius(), deltaTime);

                if (next.r <= 0.0f) { continueStep = false; return; }

                //given the calculate state, update it to this one
                r = next.r;
                phi = next.phi;
                dr = next.dr;
                dphi = next.dphi;

                //convert from polar to carestian coords
                position.x = bh.position.x + r * std::cos(phi);
                position.y = bh.position.y + r * std::sin(phi);
            }

            trail.push_back(glm::vec2(position.x, position.y));

            if (trail.size() > 400)
                trail.erase(trail.begin());
        }
    };
    //cartesian coordinates that calculate velcoity adn acceleration based on multiple black holes at a time
    struct lightRayCartesian
    {
        //the basic stuff for rendering
        glm::vec3 position;
        glm::vec3 color;
        glm::vec2 direction;

        glm::vec2 velocity;
        float speed = 200.0f; //supposed to be the speed of light but 200 for visual purposes

        std::vector<glm::vec2> trail;
        bool continueStep = true;
        bool m_intialized = false;

        //constructor
        lightRayCartesian(glm::vec3 pos, glm::vec3 color, glm::vec2 dir)
            : position(pos), color(color), direction(glm::normalize(dir))
        {
        }

        //compute geodesic accerleration when taking into account all black holes in the cartesian space, not just a single one in polar space
        glm::vec2 computeAcceleration(glm::vec2 pos, glm::vec2 velocity, const std::vector<blackHole2D>& blackholes)
        {
            glm::vec2 totalAcceleration(0.0f, 0.0f);

            for (const blackHole2D& bh : blackholes)
            {
                //calculate distance between ray and bh
                glm::vec2 bhPos(bh.position.x, bh.position.y);
                glm::vec2 offset = pos - bhPos;
                float radius = glm::length(offset);

                //skip if they are too close
                if (radius < 0.01f) { continue; }
                float r_s = bh.getRenderRadius();

                //bending component formula: a_perp = (rs/ r^2) * v^2 * n_perp
                //radial component gets scaled by 1 - rs/r
                //this calculates the unit vector away from the black hole
                glm::vec2 radial = offset / radius;

                float v2 = glm::dot(velocity, velocity);
                float vr = glm::dot(velocity, radial); //the radial velocity, essentialy the velocity of the bend

                //geodesic acceleration, made into cartesian, with a strength factor for visual effect
                float strength = 1.2f;
                float factor = r_s / (2.0f * radius * radius) * strength;

                //calculate acceleration via the radial pull as well as the conservation of angular momentum
                glm::vec2 acceleration = -factor * (v2 + vr * vr) * radial;

                totalAcceleration += acceleration;
            }

            return totalAcceleration;
        }

        void step(float deltaTime, const std::vector<blackHole2D>& blackholes, float m_speed)
        {
            //assurance for mitigating light rays from steping into bh
            //if (!continueStep) { return; }

            speed = m_speed;
            glm::vec2 pos2(position.x, position.y);

            //update velocity values on first run
            if (!m_intialized) { velocity = glm::normalize(direction) * speed; m_intialized = true; }

            //if (trail.size() > 1 && )
            //    trail.erase(trail.begin());

            for (const blackHole2D bh : blackholes)
            {
                //collision detection
                glm::vec2 bhPos(bh.position.x, bh.position.y);
                float dist = glm::length(pos2 - bhPos);
                if (dist <= bh.getRenderRadius())
                {
                    //continueStep = false;
                    return;
                }
            }

            //lambda function found on the internet for simple rk4 stepping
            auto deriv = [&](glm::vec2 p, glm::vec2 v) -> std::pair<glm::vec2, glm::vec2>
            {
                    glm::vec2 accleration = computeAcceleration(p, v, blackholes);
                    return { v, accleration };
            };

            //r4k steps 1-4
            std::pair<glm::vec2, glm::vec2> k1 = deriv(pos2, velocity);
            glm::vec2 dp1 = k1.first, dv1 = k1.second;

            std::pair<glm::vec2, glm::vec2> k2 = deriv(pos2 + dp1 * (deltaTime / 2), velocity + dv1 * (deltaTime/2));
            glm::vec2 dp2 = k2.first, dv2 = k2.second;

            std::pair<glm::vec2, glm::vec2> k3 = deriv(pos2 + dp2 * (deltaTime / 2.0f), velocity + dv2 * (deltaTime / 2.0f));
            glm::vec2 dp3 = k3.first, dv3 = k3.second;

            std::pair<glm::vec2, glm::vec2> k4 = deriv(pos2 + dp3 * deltaTime, velocity + dv3 * deltaTime);
            glm::vec2 dp4 = k4.first, dv4 = k4.second;



            //update the position and velocity based on the average of all 4 steps, weighted on the first and second step
            pos2     += (deltaTime / 6.0f) * (dp1 + 2.0f * dp2 + 2.0f * dp3 + dp4);
            velocity += (deltaTime / 6.0f) * (dv1 + 2.0f * dv2 + 2.0f * dv3 + dv4);

            velocity = glm::normalize(velocity) * speed; //normalize because c is a constant

            //internal vals updated and pushed back
            position.x = pos2.x;
            position.y = pos2.y;

            trail.push_back(pos2);
            if (trail.size() > 400)
                trail.erase(trail.begin());
        }
    };


private:
    std::vector<blackHole2D> m_blackHoles;
    std::vector<lightRayCartesian>    m_lightRaysC;
    std::vector<lightRayPolar>        m_lightRaysP;
    double m_renderScale = 1e-11;   //to scale things down from meters to pixels

    //coesmetic stuff
    std::map<int, std::pair<std::string, double>> m_bhData = { 
        {0, { "Messier 87",    4.8e42}},
        {1, { "Sagittarius A", 8.54e36}},
        {2, { "TON 618",       1.31e41}},
        {3, { "Cygnus X-1",    4.21e31}},
        {4, { "Gaia BH1",      1.9e31}}
    };
    std::string m_bhTextureData[3] = {"BlackHole2D", "BlackHole2D-Photon", "BlackHoleSide"};

    //imgui bool set stuff
    bool                     m_cameraIsStatic = true;
    bool                     m_openGUI = false;
    bool                     m_usePolar = false;
    bool                     m_staticDrawRays = true;
    bool                     m_contiousSpawn = false;
    float                    m_lightSpeed = 200.0f;
    float                    m_attractionStrength = 1.2f;

    glm::vec2                m_mouseStart; //needed for aim draw given i cant pass both a start and end

    int                      m_lightClickMode = 0;
    std::map<int, int>       m_summonAmounts = { {0, 100}, {1, 128}, {2, 1} }; //side/top, circle, aim
    std::unique_ptr<Camera>  m_camera;



public:
    BlackHole2D_Scene(Engine& gameEngine);

    //inherited functions
    void init();
    void update() override;
    void sRender() override;
    void sUserInput(const Action& action) override;
    void sGUI();
    void onEnd() override;

    void drawStraightRays(int num, int direction);
    int  checkSideIntersection(float mouseX, float mouseY);
    void drawCircularRays(glm::vec2 origin, unsigned int count);
    void drawAimedRay(glm::vec2 startPos, glm::vec2 endPos);

    void checkBorderCollision();

};