#include "shader.h"

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
	if (NULL == shader_src)
		return (0);
	id = glCreateShader(type);
	glShaderSource(id, 1, (const GLchar *const *)&shader_src, NULL);
	glCompileShader(id);
	/* Error handling */
	free(shader_src);
	return (id);
}

void shader_create(shader_t* sh, const char* vert_path, const char* frag_path)
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

	sh->id = program;
}

void shader_use(const shader_t* sh)
{
	glUseProgram(sh->id);
}

void shader_set_int(const shader_t* sh, const char* name, int value)
{
	if (!name)
		return ;
	GLuint loc = glGetUniformLocation(sh->id, name);
	glUniform1i(loc, value);
}

void shader_set_float(const shader_t* sh, const char* name, float value)
{
	if (!name)
		return ;
	GLuint loc = glGetUniformLocation(sh->id, name);
	glUniform1f(loc, value);
}

void shader_set_mat4(const shader_t* sh, const char* name, mat4_t value)
{
	if (!name)
		return ;
	GLuint loc = glGetUniformLocation(sh->id, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, value);
}

void shader_set_vec3(const shader_t* sh, const char* name, vec3_t value)
{
	if (!name)
		return ;
	GLuint loc = glGetUniformLocation(sh->id, name);
	glUniform3fv(loc, 1, value);
}
