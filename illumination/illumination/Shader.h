#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

/* glm�����������ͷ�ļ� */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ��װ����ɫ���࣬�����㡢ƬԪ��ɫ�������Ӧ����ɫ������
class Shader {
	GLuint program;

public:
	// ���캯�����������ļ��ж�ȡGLSL���룬���붥����ɫ����ƬԪ��ɫ����Ȼ�󴴽���������ɫ������
	Shader(const char* vertexPath, const char* fragmentPath) {
		std::string vertexCode, fragmentCode;
		std::ifstream vShaderFile, fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			// ���ļ�
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// ���ļ��������е����ݶ�ȡ��stringstream��
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// �ر��ļ�
			vShaderFile.close();
			fShaderFile.close();
			// stringstreamת��Ϊstring
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		GLuint vertex, fragment;
		// ���붥����ɫ��
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileOrLinkingErrors(vertex, "VERTEX");

		// ����ƬԪ��ɫ��
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileOrLinkingErrors(fragment, "FRAGMENT");

		// ������ɫ������
		program = glCreateProgram();
		glAttachShader(program, vertex);
		glAttachShader(program, fragment);
		glLinkProgram(program);
		checkCompileOrLinkingErrors(program, "PROGRAM");

		// ɾ����ɫ������
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	// ��ȡ��ɫ������ID
	GLuint getProgramID() const {
		return program;
	}

	// ʹ����ɫ������
	void use() {
		glUseProgram(program);
	}

	// ����uniform������ֵ
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
		// ����
		// glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &matrixValue[0][0]);
	}

private:
	void checkCompileOrLinkingErrors(GLuint shader, std::string type) {
		GLint state;
		char* infoLog;
		GLsizei len;
		if (type != "PROGRAM") { // ����/ƬԪ��ɫ���ı���
			glGetShaderiv(shader, GL_COMPILE_STATUS, &state);
			if (!state) {
				
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

				infoLog = new char[len + 1];
				glGetShaderInfoLog(shader, len, &len, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else { // ��ɫ�����������
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
