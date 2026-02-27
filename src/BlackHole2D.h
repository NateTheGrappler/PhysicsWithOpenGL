#pragma once
#include "Scene.h"
#include "cmath"

class BlackHole2D_Scene : public Scene
{
public:
    struct blackHole2D
    {
        double r_s; //the event horizon (Schwarzschild radius_
        double mass;
        glm::vec3 position;
        glm::vec3 color;
        double renderScale;


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

    struct lightRay
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

        lightRay(glm::vec3 pos, glm::vec3 color, glm::vec2 dir)
            :position(pos), color(color), direction(dir)
        {
        }

        rayState calculateDerivatives(const rayState& state, float r_s)
        {
            rayState derivedState;

            //just update regular vals with their derivatives
            derivedState.r   = state.dr;  // dr/dλ = dr
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
        void step(float deltaTime, const std::vector<blackHole2D>& blackHoles)
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

                    float speed = 200.0f; //supposed to be the speed of light, but just set a value to look nice


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

private:
    std::vector<blackHole2D> m_blackHoles;
    std::vector<lightRay>    m_lightRays;
    double m_renderScale = 1e-11;   //to scale things down from meters to pixels

    bool                     m_cameraIsStatic = true;
    std::unique_ptr<Camera>  m_camera;



public:
    BlackHole2D_Scene(Engine& gameEngine);

    //inherited functions
    void init();
    void update() override;
    void sRender() override;
    void sUserInput(const Action& action) override;
    void onEnd() override;

    void drawStraightRays(int num);
    void drawCircularRays(glm::vec2 origin, unsigned int count);



};