#pragma once

#include "plane.h"
#include "sphere.h"
#include "cylinder.h"
#include "capsule.h"
#include "quad.h"
#include "box.h"
#include "disk.h"

namespace Physics
{
	// Intersection from a segment
	bool IntersectSegmentQuad(const vec3& A, const vec3& B, const Quad& quad, vec3& interPt, vec3& interNornal);
	bool IntersectSegmentBox(const vec3& A, const vec3& B, const Box& box, vec3& interPtBox, vec3& interNormal);
	bool IntersectSegmentPlane(const vec3& A, const vec3& B, const Plane& plane, vec3& interPt, vec3& interNormal);
	bool IntersectLinePlane(const vec3& A, const vec3& B, const Plane& plane, vec3& interPt, vec3& interNormal);
	bool IntersectSegmentDisk(const vec3& A, const vec3& B, const Disk& disk, vec3& interPt, vec3& interNormal);
	bool IntersectSegmentSphere(const vec3& A, const vec3& B, const Sphere& sphere, vec3& interPts, vec3& interNormals);
	bool IntersectSegmentCylinderInf(const Cylinder& cylinder, const vec3& ptA, const vec3& ptB, vec3& interPt, vec3& interNormal);
	bool IntersectSegmentCylinder(const Cylinder& cylinder, const vec3& ptA, const vec3& ptB, vec3& interPt, vec3& interNormal);
	bool IntersectSegmentCapsule(const Capsule& caps, const vec3& ptA, const vec3& ptB, vec3& interPt, vec3& interNormal);

	// Intersections from a sphere
	bool IntersectSphereBox(const Sphere& sphere, const vec3& newSpherePos, const Box& box, vec3& interPt, vec3& interNormal);

	// Intersections from a point
	bool IntersectPointBox(const vec3& point, const Box& box);
}