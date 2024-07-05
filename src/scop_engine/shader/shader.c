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
	if (!buff) exit(-1);
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

	int success;
	char info_log[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(id, sizeof(info_log), NULL, info_log);
		printf("SHADER COMPILATION ERROR: [%s] | FILE: [%s]\n", info_log, path);
		free(shader_src);
		return (0);
	}
	free(shader_src);
	return (id);
}

shader_t* shader_create(const char* vert_path, const char* frag_path)
{
	shader_t* sh = (shader_t*)malloc(sizeof(shader_t));
	if (!sh) exit(-1);
	GLuint vert_id, frag_id, program;

	vert_id = shader_compile_(GL_VERTEX_SHADER, vert_path);
	frag_id = shader_compile_(GL_FRAGMENT_SHADER, frag_path);

	if (!vert_id || !frag_id)
	{
		free(sh);
		return NULL;
	}

	program = glCreateProgram();
	glAttachShader(program, vert_id);
	glAttachShader(program, frag_id);
	glLinkProgram(program);

	int success;
	char info_log[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, sizeof(info_log), NULL, info_log);
		printf("SHADER LINKING ERROR: [%s]\n", info_log);
		glDeleteShader(vert_id);
		glDeleteShader(frag_id);
		return (0);
	}

	/* Error handling */
	glDeleteShader(vert_id);
	glDeleteShader(frag_id);

	sh->id = program;
	return sh;
}

void shader_destroy(shader_t* sh)
{
	if (!sh) return;
	glDeleteProgram(sh->id);
	free(sh);
}

void shader_use(const shader_t* sh)
{
	if (!sh) return;
	glUseProgram(sh->id);
}

void shader_set_int(const shader_t* sh, const char* name, int value)
{
	if (!name || !sh)
		return ;
	shader_use(sh);
	GLuint loc = glGetUniformLocation(sh->id, name);
	glUniform1i(loc, value);
}

void shader_set_float(const shader_t* sh, const char* name, float value)
{
	if (!name || !sh)
		return ;
	shader_use(sh);
	GLuint loc = glGetUniformLocation(sh->id, name);
	glUniform1f(loc, value);
}

void shader_set_mat4(const shader_t* sh, const char* name, mat4_t value)
{
	if (!name || !sh)
		return ;
	shader_use(sh);
	GLuint loc = glGetUniformLocation(sh->id, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, value);
}

void shader_set_vec3(const shader_t* sh, const char* name, vec3_t value)
{
	if (!name || !sh)
		return ;
	shader_use(sh);
	GLuint loc = glGetUniformLocation(sh->id, name);
	glUniform3fv(loc, 1, value);
}
