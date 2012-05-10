/**> HEADER FILES <**/
#include "Camera.h"


void Camera::Apply()
{
	glTranslatef(-position.x,-position.y,-position.z);
}

