#include "shader.hh"
#include <fstream>
#include "debug.hh"
#include <sstream>

shader_rep::shader_rep(std::string code, GLenum type) :
        globject(glCreateShader(type)),
        refcount(1)
{
        const char *c_code = code.c_str(); // c-style string for shader code
        GLint status;                      // captures shader compile status
        std::string errmsg;                // holds possible compile errmsg

        // check objects
        check(globject != 0);

        // source and compile shader code
        glShaderSource(globject, 1, &c_code, nullptr);
        glCompileShader(globject);

        // check for compile errors
        glGetShaderiv(globject, GL_COMPILE_STATUS, &status);

        // throw if any exist
        if (status == GL_FALSE) {

                errmsg = compiler_errmsg(globject);
                glDeleteShader(globject);
                throw std::runtime_error(errmsg);
        }
}

shader_rep::~shader_rep()
{
        glDeleteShader(globject);
}

shader::shader(std::string code, GLenum type) :
        rep(new shader_rep(code, type))
{
}

shader::~shader()
{
        if (rep != nullptr && --rep->refcount <= 0) {
                delete rep;
                rep = nullptr;
        }
}

/*
 * Non-throwing swap function.
 */
void swap(shader& a, shader& b)
{
        std::swap(a.rep, b.rep);
}

/*
 * "Copy-and-swap" copy constructor.
 */
shader::shader(const shader& other) : rep(other.rep)
{
        rep->refcount++;
}

/*
 * "Copy-and-swap" copy assignment operator.
 */
shader& shader::operator=(shader other)
{
        swap(*this, other);
        return *this;
}

/*
 * Retrieve the managed resource.
 */
const GLuint& shader::get() const
{
        return rep->globject;
}

/*
 * Compose error message to throw (helper function).
 */
std::string shader_rep::compiler_errmsg(GLuint globject)
{
        char *log_str;          // C style log info string
        GLint log_len;          // Info log total length
        std::string errmsg;     // Composed error msg

        errmsg = std::string("Shader compile failure: ");
        glGetShaderiv(globject, GL_INFO_LOG_LENGTH, &log_len);

        log_str = new char[log_len + 1];
        glGetShaderInfoLog(globject, log_len, nullptr, log_str);

        errmsg += log_str;
        delete[] log_str;

        return errmsg;
}

/*
 * Instantiate shader from source file.
 */
shader shader_from_file(const std::string&& path, GLenum type)
{
        std::ifstream handle(path, std::ios::in | std::ios::binary);
        check(handle.is_open());

        std::stringstream buffer;
        buffer << handle.rdbuf();

        put("shader path: %s\n", path.c_str());

        return shader(buffer.str(), type);
}
