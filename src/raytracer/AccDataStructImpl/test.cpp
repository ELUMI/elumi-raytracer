

#include "glm/glm.hpp"
#include "../utilities/Triangle.h"

using namespace glm;

namespace raytracer {

int main(int argc, char* argv[]) {
  vec3 verts[] = { vec3(-1, -1, 5), vec3(1, -1, 5), vec3(0, 1, 5) };

  Triangle tri = Triangle(verts, NULL, NULL, NULL);
}

}
