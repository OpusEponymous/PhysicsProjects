#include "Derivative.h"

Derivative::Derivative(){

}

Derivative::Derivative(glm::vec3 vel, glm::quat sp, glm::vec3 f, glm::vec3 tor){
	velocity = vel;
	spin = sp;
	force = f;
	torque = tor;
}