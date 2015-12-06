/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		implements light_source.h

***********************************************************/

#include <cmath>
#include "light_source.h"

void PointLight::shade( Ray3D& ray, Vector3D view ) {
	// TODO: implement this function to fill in values for ray.col 
	// using phong shading.  Make sure your vectors are normalized, and
	// clamp colour values to 1.0.
	//
	// It is assumed at this point that the intersection information in ray 
	// is available.  So be sure that traverseScene() is called on the ray 
	// before this function.  

	float shade = ray.intersection.normal.dot(ray.dir);

	// the ray's colour will be (i) the ambient coefficient at the 
	// intersection point added to (ii) the diffuse coefficient at the 
	// point after being multiplied with the shade value

	// using an ambient starting point of 0.3 and calculating the 
	// diffuse coefficient using the shade value calculated above
	Colour ambient = (_col_ambient * ray.intersection.mat->ambient) + (0.1 * ray.intersection.mat->ambient);
	Colour diffuse = shade * _col_diffuse * ray.intersection.mat->diffuse;

	Vector3D reflectedRay = 2 * shade * ray.intersection.normal - ray.dir;
	reflectedRay.normalize();
	
	double intensity = reflectedRay.dot(view) > 0.0f ? reflectedRay.dot(view) : 0.0f;
	Colour specular = pow(intensity, 60.0) * ray.intersection.mat->specular * _col_specular;
	ray.col = ambient + diffuse + specular;

	ray.col.clamp();
}