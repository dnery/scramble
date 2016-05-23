/*
 * Created by danilo on 3/31/16.
 */

#include <fstream>
#include <sstream>
#include "debug.hh"
#include "shader.hh"

/*
 * Non-trivial constructor.
 */
scramble::shader_rep::shader_rep(std::string code, GLenum type) :
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

/*
 * Non-trivial destructor.
 */
scramble::shader_rep::~shader_rep()
{
        glDeleteShader(globject);
}

/*
 * Non-trivial constructor.
 */
scramble::shader::shader(std::string code, GLenum type) :
        rep(new shader_rep(code, type))
{
}

/*
 * Non-trivial destructor.
 */
scramble::shader::~shader()
{
        if (rep != nullptr && --rep->refcount <= 0) {

                delete rep;
                rep = nullptr;
        }
}

/*
 * Non-throwing swap function.
 */
void scramble::swap(scramble::shader& a, scramble::shader& b)
{
        std::swap(a.rep, b.rep);
}

/*
 * "Copy-and-swap" copy constructor.
 */
scramble::shader::shader(const shader& other) :
        rep(other.rep)
{
        rep->refcount++;
}

/*
 * "Copy-and-swap" copy assignment operator.
 */
scramble::shader& scramble::shader::operator=(shader other)
{
        swap(*this, other);
        return *this;
}

/*
 * Retrieve the managed resource.
 */
GLuint scramble::shader::get() const
{
        return rep->globject;
}

/*
 * Compose error message to throw (helper function).
 */
std::string scramble::compiler_errmsg(GLuint globject)
{
        char *log_str;      // C style log info string
        GLint log_len;      // Info log total length
        std::string errmsg; // Composed error msg

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
scramble::shader scramble::shader_from_file(const std::string&& path, GLenum type)
{
        std::ifstream handle(path, std::ios::in | std::ios::binary);
        check(handle.is_open());

        std::stringstream buffer;
        buffer << handle.rdbuf();

        put("shader path: %s\n", path.c_str());
        //unsc_logmsg("shader sauce:\n%s\n", buffer.str().c_str());

        return scramble::shader(buffer.str(), type);
}
