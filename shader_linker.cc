//
// Created by danilo on 4/1/16.
//

#include <stdexcept>
#include <GL/glew.h>

#include "unscramble.hh"
#include "shader_loader.hh"
#include "shader_linker.hh"

/*
 * Default constructor
 */
scramble::program::program() :
        globject(0)
{}

/*
 * Compose error message to throw (helper function).
 */
std::string linker_errmsg(GLuint globject)
{
        char        *log_str; // C style log info string
        GLint       log_len;  // Info log total length
        std::string errmsg;   // Composed error msg

        errmsg = std::string("Program linking failure: ");
        glGetProgramiv(globject, GL_INFO_LOG_LENGTH, &log_len);

        log_str = new char[log_len + 1];
        glGetProgramInfoLog(globject, log_len, nullptr, log_str);

        errmsg += log_str;
        delete[] log_str;

        return errmsg;
}

/*
 * Non-trivial constructor.
 */
scramble::program::program(const std::vector<scramble::shader>& shaders) :
        globject(glCreateProgram())
{
        GLint       status; // captures program link status
        std::string errmsg; // holds possible link errmsg

        // check objects
        unsc_assert(globject != 0);
        unsc_assert(shaders.size() > 0);

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

                errmsg = linker_errmsg(globject);
                glDeleteProgram(globject);
                throw std::runtime_error(errmsg);
        }
}

/*
 * Non-trivial destructor.
 */
scramble::program::~program()
{
        glDeleteProgram(globject);
}

/*
 * Retrieve the managed resource.
 */
GLuint scramble::program::get() const
{
        return globject;
}

/*
 * Get resource attrib location via glGetAttribLocation.
 */
GLint scramble::program::attrib(const GLchar *name) const
{
        GLint index; // holds attribute index

        unsc_assert(name != nullptr);

        index = glGetAttribLocation(globject, name);

        unsc_assert(index != -1);

        return index;
}

/*
 * Get resource uniform location via glGetUniformLocation.
 */
GLint scramble::program::uniform(const GLchar *name) const
{
        GLint index; // holds uniform index

        unsc_assert(name != nullptr);

        index = glGetUniformLocation(globject, name);

        unsc_assert(index != -1);

        return index;
}