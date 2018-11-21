#include "config.h"
#include "shaderMethods.h"

GLuint loadShaderProg(const char *vertexFilePath, const char *fragmentFilePath, const char *geometryFilePath) {
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	
	// Read the Vertex Shader code from the file
	std::string vertexShaderCode;
	std::ifstream vertexShaderStream(vertexFilePath, std::ios::in);
	if (vertexShaderStream.is_open()) {
		std::string line = "";
		while (getline(vertexShaderStream, line))
			vertexShaderCode += "\n" + line;
		vertexShaderStream.close();
	} else {
		printf("Impossible to open %s. Check to make sure the file exists and you passed in the right filepath!\n", vertexFilePath);
		printf("The current working directory is:");
		// Please for the love of whatever deity/ies you believe in never do something like the next line of code,
		// Especially on non-Windows systems where you can have the system happily execute "rm -rf ~"
#ifdef _WIN32
		system("CD");
#else
		system("pwd");
#endif
		getchar();
		return 0;
	}
	
	// Read the Fragment Shader code from the file
	std::string fragmentShaderCode;
	std::ifstream fragmentShaderStream(fragmentFilePath, std::ios::in);
	if (fragmentShaderStream.is_open()) {
		std::string line = "";
		while (getline(fragmentShaderStream, line))
			fragmentShaderCode += "\n" + line;
		fragmentShaderStream.close();
	}

	GLint result = GL_FALSE;
	int infoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertexFilePath);
	char const *vertexSourcePointer = vertexShaderCode.c_str();
	glShaderSource(vertexShaderID, 1, &vertexSourcePointer, nullptr);
	glCompileShader(vertexShaderID);
	
	// Check Vertex Shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(vertexShaderID, infoLogLength, 0, &vertexShaderErrorMessage[0]);
		printf("%s\n", &vertexShaderErrorMessage[0]);
	} else {
		printf("Successfully compiled vertex shader!\n");
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragmentFilePath);
	char const *fragmentSourcePointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer, nullptr);
	glCompileShader(fragmentShaderID);
	
	// Check Fragment Shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(fragmentShaderID, infoLogLength, nullptr, &fragmentShaderErrorMessage[0]);
		printf("%s\n", &fragmentShaderErrorMessage[0]);
	} else {
		printf("Successfully compiled fragment shader!\n");
	}

	GLuint geometryShaderID = 0;
	if (geometryFilePath) {
		// Read the Fragment Shader code from the file
		std::string sourceCode;
		std::ifstream fstream(geometryFilePath, std::ios::in);
		if (fstream.is_open()) {
			std::string line = "";
			while (getline(fstream, line))
				sourceCode += "\n" + line;
			fstream.close();
		}
		
		geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
		
		// Compile geometry shader
		printf("Compiling shader : %s\n", geometryFilePath);
		const char *sourceCodeCStr = sourceCode.c_str();
		glShaderSource(geometryShaderID, 1, &sourceCodeCStr, nullptr);
		glCompileShader(geometryShaderID);

		// Check for errors
		glGetShaderiv(geometryShaderID, GL_COMPILE_STATUS, &result);
		glGetShaderiv(geometryShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if(infoLogLength > 0) {
			std::vector<char> errorMsg(infoLogLength + 1);
			glGetShaderInfoLog(geometryShaderID, infoLogLength, nullptr, &errorMsg[0]);
			printf("%s\n", &errorMsg[0]);
		} else {
			printf("Successfully compiled geometry shader!\n");
		}
	}

	// Link the program
	printf("Linking program\n");
	GLuint progID = glCreateProgram();
	glAttachShader(progID, vertexShaderID);
	glAttachShader(progID, fragmentShaderID);
	if (geometryShaderID != 0) glAttachShader(progID, geometryShaderID);
	glLinkProgram(progID);
	
	// Check the program
	glGetProgramiv(progID, GL_LINK_STATUS, &result);
	glGetProgramiv(progID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> progErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(progID, infoLogLength, nullptr, &progErrorMessage[0]);
		printf("%s\n", &progErrorMessage[0]);
	}
	
	glDetachShader(progID, vertexShaderID);
	glDeleteShader(vertexShaderID);

	glDetachShader(progID, fragmentShaderID);
	glDeleteShader(fragmentShaderID);

	if (geometryShaderID != 0) {
		glDetachShader(progID, geometryShaderID);
		glDeleteShader(geometryShaderID);
	}
	
	return progID;
}