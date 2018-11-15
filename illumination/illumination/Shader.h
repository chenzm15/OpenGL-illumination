#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

/* glm库的三个常用头文件 */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// 封装的着色器类，管理顶点、片元着色器及其对应的着色器程序
class Shader {
	GLuint program;

public:
	// 构造函数，从两个文件中读取GLSL代码，编译顶点着色器和片元着色器，然后创建并链接着色器程序
	Shader(const char* vertexPath, const char* fragmentPath) {
		std::string vertexCode, fragmentCode;
		std::ifstream vShaderFile, fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			// 打开文件
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// 将文件缓冲区中的内容读取到stringstream中
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// 关闭文件
			vShaderFile.close();
			fShaderFile.close();
			// stringstream转化为string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		GLuint vertex, fragment;
		// 编译顶点着色器
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileOrLinkingErrors(vertex, "VERTEX");

		// 编译片元着色器
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileOrLinkingErrors(fragment, "FRAGMENT");

		// 链接着色器程序
		program = glCreateProgram();
		glAttachShader(program, vertex);
		glAttachShader(program, fragment);
		glLinkProgram(program);
		checkCompileOrLinkingErrors(program, "PROGRAM");

		// 删除着色器对象
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	// 获取着色器程序ID
	GLuint getProgramID() const {
		return program;
	}

	// 使用着色器程序
	void use() {
		glUseProgram(program);
	}

	// 设置uniform变量的值
	// ------------------------------------------------------------------------
	void setBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(program, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(program, name.c_str()), value);
	}
	void setVec3(const std::string &name, const glm::vec3 &vecValue) const
	{
		glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, glm::value_ptr(vecValue));
	}
	void setVec3(const std::string &name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(program, name.c_str()), x, y, z);
	}
	void setMat4(const std::string &name, const glm::mat4 &matrixValue) const
	{
		glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrixValue));
		// 或者
		// glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &matrixValue[0][0]);
	}

private:
	void checkCompileOrLinkingErrors(GLuint shader, std::string type) {
		GLint state;
		char* infoLog;
		GLsizei len;
		if (type != "PROGRAM") { // 顶点/片元着色器的编译
			glGetShaderiv(shader, GL_COMPILE_STATUS, &state);
			if (!state) {
				
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

				infoLog = new char[len + 1];
				glGetShaderInfoLog(shader, len, &len, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else { // 着色器程序的链接
			glGetProgramiv(shader, GL_LINK_STATUS, &state);
			if (!state) {
				glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &len);

				infoLog = new char[len + 1];
				glGetProgramInfoLog(shader, len, &len, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};

#endif
