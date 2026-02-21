#pragma once
#ifndef SHADER
#define SHADER
#define GLFW_INCLUDE_NONE

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct ShaderPrograms
{
    std::string VertexSource;
    std::string FragemntSource;
};

class Shader
{
public:
    //public valued
    unsigned int ID;

    //--------------------------------------function defintions--------------------------------------
    void use() const
    {
        glUseProgram(ID);
    }
    void setUniformBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    void setUniformInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setUniformFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    //-----------------------------------------------------------------------------
    void setVec2(const std::string& name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    //--------------------------------------------------------------------------------------
    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }


    //--------------------------------------constructor--------------------------------------
    Shader(const std::string& shaderPath)
    {
        //enum class to store the shader types
        enum class ShaderType
        {
            NONE = -1, VERTEX = 0, FRAGEMENT = 1
        };


        //set up filepath as well as buffer to toss string data into
        std::ifstream stream(shaderPath);
        std::stringstream ss[2];
        ShaderType type = ShaderType::NONE;


        //read file line by line
        std::string line;
        while (getline(stream, line))
        {

            //check to see if the .find() function finds the position of said string
            if (line.find("#shader") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                {
                    //set as vertex shader
                    type = ShaderType::VERTEX;
                }
                else if (line.find("fragement") != std::string::npos)
                {
                    //set as fragment shader
                    type = ShaderType::FRAGEMENT;
                }
            }
            else
            {
                //at given shader type, index into array
                ss[(int)type] << line << '\n';
            }
        }

        _shaderSources = { ss[0].str(), ss[1].str() };

        //now call the creation of the actual shader program
        ID = createShader(_shaderSources.VertexSource, _shaderSources.FragemntSource);
    }

private:
    ShaderPrograms _shaderSources;

    //--------------------------------------static helper functions--------------------------------------
    static unsigned int compileShader(unsigned int type, const std::string& source)
    {
        //create the shader and compile it given the type of shader it is and the source code of the shader
        unsigned int shader = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        //use glGetShaderiv in order to check if the shader compiled correctly or not
        int result;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        if (!result)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "Failed to initialize " << (type == GL_FRAGMENT_SHADER ? "fragement" : "vertex") << " shader " << infoLog << std::endl;
            glDeleteShader(shader);
            return 0;
        }

        //return the compiled shader if successful
        return shader;
    }
    static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader)
    {
        //create the shader program, and then also compile both your vertex and frag shaders
        unsigned int program = glCreateProgram();
        unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
        unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

        if (vs == 0 || fs == 0)
        {
            glDeleteShader(vs);
            glDeleteShader(fs);
            return 0;
        }

        //link the compiled shaders to the shader program, then compile shaderprogram
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);

        int success;
        char infoLog[512];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            std::cerr << "ERROR: Shader program linking failed:\n" << infoLog << std::endl;
            glDeleteProgram(program);
            program = 0;
        }

        //delete the now uneeded vertex and fragement shaders as they are stored in program
        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }
};


#endif 
