/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		implements scene_object.h

***********************************************************/

#include <cmath>
#include <iostream>
#include "scene_object.h"

bool UnitSquare::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld ) {
	// TODO: implement intersection code for UnitSquare, which is
	// defined on the xy-plane, with vertices (0.5, 0.5, 0), 
	// (-0.5, 0.5, 0), (-0.5, -0.5, 0), (0.5, -0.5, 0), and normal
	// (0, 0, 1).
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point, 
	// intersection.normal, intersection.none, intersection.t_value.   
	//
	// HINT: Remember to first transform the ray into object space  
	// to simplify the intersection test.

	if (!ray.intersection.none) {
		return false;
	}

	// the normal vector to the plane
	Vector3D n = Vector3D(0.0, 0.0, 1.0);

	// points on the plane
	Point3D p1 = Point3D(0.5, 0.5 ,0);
	Point3D p2 = Point3D(-0.5, 0.5 ,0);
	Point3D p3 = Point3D(-0.5, -0.5 ,0);
	Point3D p4 = Point3D(0.5, -0.5 ,0);

	Ray3D rayCopy = ray;

	// transform the ray into object space
	rayCopy.dir = worldToModel * rayCopy.dir;
	rayCopy.origin = worldToModel * rayCopy.origin;
	rayCopy.dir.normalize();

	if (rayCopy.dir[2] == 0.0f ) {
		ray.intersection.none = true;
		return false;
	} else if (rayCopy.dir[2] * rayCopy.origin[2] > 0.0f) {
		// this means, either the origin is above the plane and 
		// the direction goes away from the plane, or the origin
		// is below the plane and the direction goes away from the plane
		ray.intersection.none = true;
		return false;
	}

	// using the parametric equation for the plane, solve for t
	// x and why values cancel out since x and y values of the normal 
	// are zero, leaving only the z value
	// normal[z] * (ray.origin[z] + t*ray.dir[z])
	const double t = -(rayCopy.origin[2]/(rayCopy.dir[2]));

	if (t >= 0.0f) {
		// intersection with plane!

		// make sure there isn't already a t_value that is closer to the ray's origin
		//if ((ray.intersection.t_value &&  t < ray.intersection.t_value) || !ray.intersection.t_value) {
			//std::cout << !ray.intersection.t_value << "\n";
		//std::cout << t << "\n";
			Point3D interPoint = rayCopy.origin + t*rayCopy.dir;

			// check if intersection is inside the given boundaries or outside
			//Vector3D planeToRayOrigin = rayCopy.origin - interPoint;

			if (interPoint[0] >= -0.5 && interPoint[0] <= 0.5 && interPoint[1] >= -0.5 && interPoint[1] <= 0.5) {
				//std::cout << t << "\n";
				// inside!
				ray.intersection.t_value = t;
				ray.intersection.point = modelToWorld * interPoint;

				// transform the normal to world coords
				ray.intersection.normal = transNorm(worldToModel, -n);
				ray.intersection.normal.normalize();
				ray.intersection.none = false;
			}
		//}
	}

	return !ray.intersection.none;
}

bool UnitSphere::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld ) {
	// TODO: implement intersection code for UnitSphere, which is centred 
	// on the origin.  
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point, 
	// intersection.normal, intersection.none, intersection.t_value.   
	//
	// HINT: Remember to first transform the ray into object space  
	// to simplify the intersection test.

	if (!ray.intersection.none) {
		return false;
	}

	Ray3D rayCopy = ray;

	// transform the ray into object space
	rayCopy.dir = worldToModel * rayCopy.dir;
	rayCopy.origin = worldToModel * rayCopy.origin;
	rayCopy.dir.normalize();

	float radius = 1.0f;
	const Point3D center = Point3D(0.0f, 0.0f, 0.0f);
	
	// L is the vector from the ray's origin
	// to the center of the sphere
	Vector3D L = center - rayCopy.origin;

	// tc gives us the distance from the ray's origin 
	// to halfway between the 2 intersection points 
	// ie, in line with the sphere's origin
	//
	// if less than 0, the sphere is behind view area
	// so the ray doesn't intersect
	float tc = L.dot(rayCopy.dir);
	if (tc >= 0.0f) {
		// d is the distance from (i) the midpoint between 
		// the 2 intersection points (along the ray), 
		// to (ii) the center of the sphere
		//
		// if greater than the radius, then the ray doesn't intersect
		float d = sqrt( L.dot(L) - (tc*tc) );
		if (d <= radius) {
			// intersection!
			//
			// half the distance between both intersection points
			// ie, the midpoint between the 2 intersection points,
			// along the ray
			float t1c = sqrt((radius*radius) - (d*d));

			float t1 = tc - t1c;
			float t2 = tc + t1c;

			// there may be 2 points of intersection, 
			// but since we are dealing with 3D opaque objects,
			// we only need the closest one to the origin of the ray
			ray.intersection.t_value = (t1 <= t2 ? t1 : t2);
			ray.intersection.point = rayCopy.origin + ray.intersection.t_value*rayCopy.dir;
			ray.intersection.normal = center - ray.intersection.point;
			ray.intersection.point = modelToWorld * ray.intersection.point;
			ray.intersection.normal = transNorm(worldToModel, ray.intersection.normal);
			ray.intersection.normal.normalize();
			ray.intersection.none = false;
		}
	}

	return !ray.intersection.none;
}
