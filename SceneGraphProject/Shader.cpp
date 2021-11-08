#include "Shader.h"


Shader::Shader()
	:m_id(0)
{
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	LoadShader(vertexFile, fragmentFile);
}

void Shader::LoadShader(const char* vertexFile, const char* fragmentFile)
{
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	CompileErrors(vertexShader, "VERTEX");

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	CompileErrors(fragmentShader, "FRAGMENT");

	m_id = glCreateProgram();
	glAttachShader(m_id, vertexShader);
	glAttachShader(m_id, fragmentShader);
	glLinkProgram(m_id);
	CompileErrors(m_id, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Bind()
{
	glUseProgram(m_id);
}

void Shader::SetInt(const char* uniformName, int value) const
{
	int location = glGetUniformLocation(m_id, uniformName);
	glUniform1i(location, value);
}

void Shader::SetFloat(const char* uniformName, float value) const
{
	int location = glGetUniformLocation(m_id, uniformName);
	glUniform1f(location, value);
}

void Shader::SetVec3(const char* uniformName, glm::vec3 vec) const
{
	int location = glGetUniformLocation(m_id, uniformName);
	glUniform3fv(location, 1, &vec[0]);
}

void Shader::SetMat4(const char* uniformName, glm::mat4& mat) const
{
	int location = glGetUniformLocation(m_id, uniformName);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::SetShading(bool isEnabled)
{
	m_flags = isEnabled ? m_flags |= 0x01 : m_flags &= 0xfe;
}

bool Shader::GetShading()
{
	return m_flags & 0x01;
}

void Shader::SetCustomData(bool isEnabled)
{
	m_flags = isEnabled ? m_flags |= 0x02 : m_flags &= 0xfd;
}

bool Shader::GetCustomData()
{
	return m_flags & 0x02;
}

void Shader::AddCustomData(std::string name, UniformData data)
{
	m_data[name] = data;
}

std::unordered_map<std::string, UniformData>& Shader::GetData()
{
	return m_data;
}

void Shader::SetTimeInput(bool isEnabled)
{
	m_flags = isEnabled ? m_flags |= 0x04 : m_flags &= 0xfb;
}

bool Shader::GetTimeInput()
{
	return m_flags & 0x4;
}

void Shader::CompileErrors(unsigned int shader, const char* type)
{
	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "<<Shader>> " << type << " shader compilation error:" << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "<<Shader>> Shader linking error: " << type << "\n" << infoLog << std::endl;
		}
	}
}

std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}
