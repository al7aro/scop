#include "shader_reader.h"

/*
 * Interesting notes for future me:
 * - In windows the line break has 2 chars (\r\n)
 * - It is better to use binary mode to open files
 * TODO: Study error checking for this function
 */
static char* read_file_(const char *path)
{
	FILE*	fp;
	size_t	start, end, len;
	char*	buff;

	fp = fopen(path, "rb");
	if (NULL == fp)
		return (NULL);
	start = ftell(fp);
	fseek(fp, 0, SEEK_END);
	end = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	len = end - start;
	buff = (char*)malloc(sizeof(char) * (len + 1));
	if (NULL == buff)
		return (NULL);
	fread(buff, len, 1, fp);
	buff[len] = 0;
	fclose(fp);
	return (buff);
}

GLuint shader_compile_(GLuint type, const char* path)
{
	GLuint id;
	char* shader_src;

	shader_src = read_file_(path);
	printf("Shader: %s\n", shader_src);
	if (NULL == shader_src)
		return (-1);
	id = glCreateShader(type);
	glShaderSource(id, 1, &shader_src, NULL);
	glCompileShader(id);
	/* Error handling */
	free(shader_src);
	return (id);
}

GLuint shader_create(const char* vert_path, const char* frag_path)
{
	GLuint vert_id, frag_id, program;

	vert_id = shader_compile_(GL_VERTEX_SHADER, vert_path);
	frag_id = shader_compile_(GL_FRAGMENT_SHADER, frag_path);

	program = glCreateProgram();
	glAttachShader(program, vert_id);
	glAttachShader(program, frag_id);
	glLinkProgram(program);
	/* Error handling */
	glDeleteShader(vert_id);
	glDeleteShader(frag_id);

	return (program);
}