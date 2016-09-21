#include "gldecs.h"
#include "gldraw.h"

#include "GLM\glm.hpp"
#include "GLM\ext.hpp"

void clearFrameBuffer(const FrameBuffer & r)
{
	glBindFramebuffer(GL_FRAMEBUFFER, r.handle);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void tdraw_internal::tdraw_begin(const Shader & s, const Geometry & g, const FrameBuffer & r)
{
	glBindFramebuffer(GL_FRAMEBUFFER, r.handle);
	glUseProgram(s.handle);
	glBindVertexArray(g.vao);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, r.width, r.height);
}


void tdraw_internal::tdraw_close(const Shader & s, const Geometry & g, const FrameBuffer & r)
{
	glDrawElements(GL_TRIANGLES, g.size, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

size_t tdraw_internal::tdraw_format(size_t idx, size_t tex, const glm::mat4 & val)
{
	glUniformMatrix4fv(idx, 1, GL_FALSE, glm::value_ptr(val));
	return 0;
}

size_t tdraw_internal::tdraw_format(size_t idx, size_t tex, const glm::vec3 & val)
{
	glUniform3fv(idx, 1, glm::value_ptr(val));
	return 0;
}

size_t tdraw_internal::tdraw_format(size_t idx, size_t tex, int val)
{
	glUniform1i(idx, val);
	return 0;
}

size_t tdraw_internal::tdraw_format(size_t idx, size_t tex, float val)
{
	glUniform1f(idx, val);
	return 0;
}

size_t tdraw_internal::tdraw_format(size_t idx, size_t tex, const Texture & val)
{
	glActiveTexture(GL_TEXTURE0 + tex);
	glBindTexture(GL_TEXTURE_2D, val.handle);
	glUniform1i(idx, tex);
	return 1;
}