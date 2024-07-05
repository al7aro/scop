#ifndef SCOP_MACROS_H
#define SCOP_MACROS_H

#ifdef _WIN32
	#define strtok_r strtok_s
#endif

#define SCOP_POS_ATT_ID 0
#define SCOP_TEX_ATT_ID 1
#define SCOP_NOR_ATT_ID 2
#define SCOP_PTO_ATT_ID 3
#define SCOP_COL_ATT_ID 4
#define SCOP_MAX_ATT 5

#define MAX_FACE_SIZE 64

#define SCOP_TEXTURE_ID_KD 0
#define SCOP_TEXTURE_ID_KS 1
#define SCOP_TEXTURE_ID_BUMP 2
#define SCOP_TEXTURE_ID_DEFAULT 3
#define SCOP_TEXTURE_ID_MAX 4


#define MAX_LINE_SIZE 512
#define SCOP_ERROR_INVALID_SYNTAX "Invalid syntax"
#define SCOP_ERROR_FILE_NOT_FOUND "File not found"
#define SCOP_ERROR_UNRECOGNICED_FILE "Unrecogniced file type. (Available formats: PAM)"
#define SCOP_ERROR_FATAL "Fatal error"
#define SCOP_INVALID_ARGUMENTS "Invalid arguments"

#ifndef SCOP_INSTALL_PATH
# define SCOP_INSTALL_PATH "/"
#endif

#endif
