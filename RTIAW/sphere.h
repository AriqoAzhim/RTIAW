#pragma once
#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
private:
	point3 center;
	double radius;
public:
	sphere(point3 center, double radius) : center(center), radius(radius) {}
		
	bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
		vec3 oc = r.origin() - center;
		auto a = r.direction().length_squared();
		auto half_b = dot(oc, r.direction());
		auto c = oc.length_squared() - radius * radius;
		auto discriminant = half_b * half_b - a * c;
		if (discriminant < 0) return false;

		auto sqrtd = sqrt(discriminant);

		// Find nearest root within acceptable range (within tmin - tmax)
		auto root = (-half_b - sqrtd) / a;
		if (root <= ray_t.min || ray_t.max <= root) {
			root = (-half_b + sqrtd) / a;
			if (root <= ray_t.min || ray_t.max <= root)
				return false;
		}

		rec.t = root;
		rec.p = r.at(rec.t);

		vec3 outward_normal = (rec.p - center) / radius;
		rec.set_face_normal(r, outward_normal);

		return true;

	}
};