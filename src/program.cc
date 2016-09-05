#include "program.hh"

#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include "debug.hh" // order matters, read debug.hh

program::program(const std::vector<shader>& shaders) :
        globject(glCreateProgram())
{
        GLint status;           // captures program link status
        std::string errmsg;     // holds possible link errmsg

        // check objects
        check(globject == 0);
        check(shaders.size() == 0);

        // attach shaders
        for (unsigned i = 0; i < shaders.size(); i++)
                glAttachShader(globject, shaders[i].get());

        // compose program
        glLinkProgram(globject);

        // detach shaders
        for (unsigned i = 0; i < shaders.size(); i++)
                glDetachShader(globject, shaders[i].get());

        // check for link errors
        glGetProgramiv(globject, GL_LINK_STATUS, &status);

        // throw if any exist
        if (status == GL_FALSE) {

                errmsg = error(globject);
                glDeleteProgram(globject);
                mcheck(1, "%s\n", errmsg.c_str());
        }
}

program::~program()
{
        check(globject == 0);

        glDeleteProgram(globject);
}

/*
 * Check if current proram is in use.
 */
bool program::in_use() const
{
        GLint current = 0; // current program in use by the API

        check(globject == 0);

        glGetIntegerv(GL_CURRENT_PROGRAM, &current);

        return current == static_cast<GLint>(globject);
}

/*
 * Toggle usage for the current program.
 */
void program::toggle() const
{
        if (in_use())
                glUseProgram(0);
        else
                glUseProgram(globject);
}

/*
 * Get resource attrib location via glGetAttribLocation.
 */
GLint program::attrib(const GLchar *name) const
{
        GLint index; // holds attribute index

        check(name == nullptr);

        index = glGetAttribLocation(globject, name);

        check(index == -1);

        return index;
}

/*
 * Get resource uniform location via glGetUniformLocation.
 */
GLint program::uniform(const GLchar *name) const
{
        GLint index; // holds uniform index

        check(name == nullptr);

        index = glGetUniformLocation(globject, name);

        check(index == -1);

        return index;
}

/*
 * Compose error message to throw (helper function).
 */
std::string program::error(GLuint globject)
{
        char *log_str;      // C style log info string
        GLint log_len;      // Info log total length
        std::string errmsg; // Composed error msg

        errmsg = std::string("Program linking failure: ");
        glGetProgramiv(globject, GL_INFO_LOG_LENGTH, &log_len);

        log_str = new char[log_len + 1];
        glGetProgramInfoLog(globject, log_len, nullptr, log_str);

        errmsg += log_str;
        delete[] log_str;

        return errmsg;
}

/*
 * Write signatures as macros
 */
#define ATTRIB_N_UNIFORM_SETTERS(OGL_TYPE, TYPE_PREFIX, TYPE_SUFFIX) \
\
void program::setAttrib(const GLchar* name, OGL_TYPE v0) \
        { assert(in_use()); glVertexAttrib ## TYPE_PREFIX ## 1 ## TYPE_SUFFIX (attrib(name), v0); } \
void program::setAttrib(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1) \
        { assert(in_use()); glVertexAttrib ## TYPE_PREFIX ## 2 ## TYPE_SUFFIX (attrib(name), v0, v1); } \
void program::setAttrib(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2) \
        { assert(in_use()); glVertexAttrib ## TYPE_PREFIX ## 3 ## TYPE_SUFFIX (attrib(name), v0, v1, v2); } \
void program::setAttrib(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3) \
        { assert(in_use()); glVertexAttrib ## TYPE_PREFIX ## 4 ## TYPE_SUFFIX (attrib(name), v0, v1, v2, v3); } \
\
void program::setAttrib1v(const GLchar* name, const OGL_TYPE* v) \
        { assert(in_use()); glVertexAttrib ## TYPE_PREFIX ## 1 ## TYPE_SUFFIX ## v (attrib(name), v); } \
void program::setAttrib2v(const GLchar* name, const OGL_TYPE* v) \
        { assert(in_use()); glVertexAttrib ## TYPE_PREFIX ## 2 ## TYPE_SUFFIX ## v (attrib(name), v); } \
void program::setAttrib3v(const GLchar* name, const OGL_TYPE* v) \
        { assert(in_use()); glVertexAttrib ## TYPE_PREFIX ## 3 ## TYPE_SUFFIX ## v (attrib(name), v); } \
void program::setAttrib4v(const GLchar* name, const OGL_TYPE* v) \
        { assert(in_use()); glVertexAttrib ## TYPE_PREFIX ## 4 ## TYPE_SUFFIX ## v (attrib(name), v); } \
\
void program::setUniform(const GLchar* name, OGL_TYPE v0) \
        { assert(in_use()); glUniform1 ## TYPE_SUFFIX (uniform(name), v0); } \
void program::setUniform(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1) \
        { assert(in_use()); glUniform2 ## TYPE_SUFFIX (uniform(name), v0, v1); } \
void program::setUniform(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2) \
        { assert(in_use()); glUniform3 ## TYPE_SUFFIX (uniform(name), v0, v1, v2); } \
void program::setUniform(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3) \
        { assert(in_use()); glUniform4 ## TYPE_SUFFIX (uniform(name), v0, v1, v2, v3); } \
\
void program::setUniform1v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
        { assert(in_use()); glUniform1 ## TYPE_SUFFIX ## v (uniform(name), count, v); } \
void program::setUniform2v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
        { assert(in_use()); glUniform2 ## TYPE_SUFFIX ## v (uniform(name), count, v); } \
void program::setUniform3v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
        { assert(in_use()); glUniform3 ## TYPE_SUFFIX ## v (uniform(name), count, v); } \
void program::setUniform4v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
        { assert(in_use()); glUniform4 ## TYPE_SUFFIX ## v (uniform(name), count, v); }

/*
 * Generate previously proposed declarations
 */
ATTRIB_N_UNIFORM_SETTERS(GLfloat, , f);
ATTRIB_N_UNIFORM_SETTERS(GLdouble, , d);
ATTRIB_N_UNIFORM_SETTERS(GLint, I, i);
ATTRIB_N_UNIFORM_SETTERS(GLuint, I, ui);

/*
 * Uniforms & Attribs should be set through these
 */
void program::setUniformMatrix2(const GLchar *name, const GLfloat *v,
                                          GLsizei count, GLboolean transpose)
{
        check(!in_use());
        glUniformMatrix2fv(uniform(name), count, transpose, v);
}

void program::setUniformMatrix3(const GLchar *name, const GLfloat *v,
                                          GLsizei count, GLboolean transpose)
{
        check(!in_use());
        glUniformMatrix3fv(uniform(name), count, transpose, v);
}

void program::setUniformMatrix4(const GLchar *name, const GLfloat *v,
                                          GLsizei count, GLboolean transpose)
{
        check(!in_use());
        glUniformMatrix4fv(uniform(name), count, transpose, v);
}

void program::setUniform(const GLchar *name, const glm::mat2& m,
                                   GLboolean transpose)
{
        check(!in_use());
        glUniformMatrix2fv(uniform(name), 1, transpose, glm::value_ptr(m));
}

void program::setUniform(const GLchar *name, const glm::mat3& m,
                                   GLboolean transpose)
{
        check(!in_use());
        glUniformMatrix3fv(uniform(name), 1, transpose, glm::value_ptr(m));
}

void program::setUniform(const GLchar *name, const glm::mat4& m,
                                   GLboolean transpose)
{
        check(!in_use());
        glUniformMatrix4fv(uniform(name), 1, transpose, glm::value_ptr(m));
}

void program::setUniform(const GLchar *uniformName, const glm::vec3& v)
{
        setUniform3v(uniformName, glm::value_ptr(v));
}

void program::setUniform(const GLchar *uniformName, const glm::vec4& v)
{
        setUniform4v(uniformName, glm::value_ptr(v));
}

/*
 * Instantiate shader from source file.
 */
shader shader_from_file(const std::string&& path, GLenum type)
{
        std::ifstream handle(path, std::ios::in | std::ios::binary);
        check(!handle.is_open());

        // Assert the filepath
        info("shader path is: %s\n", path.c_str());

        std::stringstream buffer;
        buffer << handle.rdbuf();

        return shader(buffer.str(), type);
}
