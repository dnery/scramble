/*
 * Created by danilo on 4/17/16.
 */

#include "engine.hh"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "ext/platform.hh"
#include "listener.hh"
#include "locator.tt"
#include "debug.hh"
#include "camera.hh"

/*
 * Abbreviations
 */
using camService = scramble::locator<scramble::camera>;

scramble::engine::engine() :
	window(),
	reps{
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f),
		glm::vec3(-2.0f, -5.0f, 15.0f),
		glm::vec3(1.5f, 2.2f, 2.5f),
		glm::vec3(3.8f, 2.0f, 12.3f),
		glm::vec3(-2.4f, 0.4f, 3.5f),
		glm::vec3(1.7f, -3.0f, 7.5f),
		glm::vec3(-1.3f, 2.0f, 2.5f),
		glm::vec3(-1.5f, -2.0f, 2.5f),
		glm::vec3(-1.5f, -0.2f, 1.5f),
		glm::vec3(1.3f, -4.0f, 1.5f),
		glm::vec3(5.3f, -1.0f, -1.5f)
	}
{
	/*
	 * GL capabilities setup
	 */
	glewExperimental = GL_TRUE;
	check(glewInit() == GLEW_OK);
	check(GLEW_VERSION_3_3 != 0);

	glViewport(0, 0, scramble::WIN_WIDTH, scramble::WIN_HEIGHT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// TODO Backface Culling!
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/*
	 * Callback functions
	 */
	glfwSetErrorCallback(scramble::callback_err);
	glfwSetKeyCallback(window._handle(), scramble::callback_keyboard);
	glfwSetScrollCallback(window._handle(), scramble::callback_scroll);
	glfwSetCursorPosCallback(window._handle(), scramble::callback_mouse);

	/*
	 * Print debug info
	 */
	window.debug_info();

	/*
	 * Provide resources
	 */
	camService::provide(new scramble::camera());

	/*
	 * Create samples
	 */
	std::vector<scramble::shader> object_shaders;
	object_shaders.push_back(scramble::shader_from_file(
		resource_path("vert_simple.glsl"), GL_VERTEX_SHADER));
	object_shaders.push_back(scramble::shader_from_file(
		resource_path("frag_simple.glsl"), GL_FRAGMENT_SHADER));

	std::vector<scramble::shader> caster_shaders;
	caster_shaders.push_back(scramble::shader_from_file(
		resource_path("vert_caster.glsl"), GL_VERTEX_SHADER));
	caster_shaders.push_back(scramble::shader_from_file(
		resource_path("frag_caster.glsl"), GL_FRAGMENT_SHADER));

	object_program = new scramble::program(object_shaders);
	caster_program = new scramble::program(caster_shaders);

	cube = new scramble::cube;
	lamp = new scramble::cube;
}

scramble::engine::~engine()
{
	delete object_program;
	delete caster_program;
	delete cube;
	delete lamp;
}

void scramble::engine::engage()
{
	while (glfwWindowShouldClose(window._handle()) == 0) {

		// Process events
		glfwPollEvents();

		// Update procedure
		update();

		// Render procedure
		render();

		// Off-screen to on-screen
		glfwSwapBuffers(window._handle());
	}
}

void scramble::engine::update()
{
	scramble::refresh_delta();
	scramble::refresh_keyput();
}

void scramble::engine::render()
{
	// CLEAR
	glClearColor(0.25f, 0.03f, 0.08f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Projection matrix is the same for all objects
	glm::mat4 proj;
	proj = glm::perspective(camService::current()->look_zoom,
	                        scramble::ASPECT_RATIO,
	                        0.1f, 1000.0f);

	// View matrix is the same for all objects
	glm::mat4 view;
	view = camService::current()->view_mat();

	// TODO Model matrix changes!
	glm::mat4 model;

	// TODO Normal matrix changes!
	glm::mat4 normat;

	// Phong shading elements
	glm::vec3 caster_pos(0.0f, 5.0f, 0.0f);
	glm::vec3 caster_color(1.0f, 0.6f, 0.8f);

	// LIGHT CASTER
	caster_program->toggle();
	lamp->bind(caster_program);

	caster_program->setUniform("proj", proj);
	caster_program->setUniform("view", view);
	caster_program->setUniform("caster_color", caster_color);

	model = glm::translate(glm::mat4(), caster_pos);
	caster_program->setUniform("model", model);

	lamp->draw(); // :D

	lamp->unbind();
	caster_program->toggle();

	// OBJECT
	object_program->toggle();
	cube->bind(object_program);

	object_program->setUniform("proj", proj);
	object_program->setUniform("view", view);
	object_program->setUniform("viewer_pos", camService::current()->position);
	object_program->setUniform("caster_pos", caster_pos);
	object_program->setUniform("caster_color", caster_color);

	// Draw several instances
	for (GLuint i = 0; i < 20; i++) {

		model = glm::translate(glm::mat4(), reps[i]);
		model = glm::rotate(model, (GLfloat) (glfwGetTime() * 0.9f + i),
		                    glm::vec3(1.0f, 1.0f, 0.0f));
		object_program->setUniform("model", model);

		normat = glm::transpose(glm::inverse(model));
		object_program->setUniform("normat", normat);

		cube->draw(); // :D
	}

	cube->unbind();
	object_program->toggle();
}
