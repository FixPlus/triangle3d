#pragma once
#include <vector>
#include <array>
#include <cmath>

namespace lingeo3D{

	const float flt_tolerance = 0.00001;                 // tolerance that will be used in calculation of coordinates (e.g. for comparing the points)
	extern float inter_area_width;                    // defines the width (counted in flt_tolerances) of INTER_AREA that used to avoid some corner cases
	enum areas_t {LEFT_SIDE, INTER_SIDE, RIGHT_SIDE}; // there are 3 areas that are defined by the line cut of plane: left, right and inter area. The last one represents the same line with defined thickness (inter_are_width)
	enum degen_t {NORMAL_DEG, LINE_DEG, POINT_DEG, INVALID_DEG};

	template<typename T>
	struct point_t {      // (x , y, z)
		T x_, y_, z_;
		point_t(T x = NAN, T y = NAN, T z = NAN);
		point_t(const point_t<T> &point);
		
		void print() const;		// prints on screen: "(x ; y)"

		bool valid() const;
		point_t<T> vector_prod(point_t<T> const &pnt) const;
		T scalar_prod(point_t<T> const &pnt) const;
		T distance(point_t<T> const &pnt) const;
		
		bool operator==(point_t<T> const &pnt) const{
			return (std::abs(x_ - pnt.x_) < flt_tolerance && std::abs(y_ - pnt.y_) < flt_tolerance && std::abs(z_ - pnt.z_) < flt_tolerance) ? true : false;
		}
		bool operator!=(point_t<T> const &pnt) const{
			return !operator==(pnt);
		}

		point_t<T> operator-(point_t<T> const &another) const{
			point_t<T> ret(x_ - another.x_, y_ - another.y_, z_ - another.z_);
			return ret;
		}

		point_t<T> operator+(point_t const &another) const{
			point_t<T> ret(x_ + another.x_, y_ + another.y_, z_ + another.z_);
			return ret;	
		}
		point_t<T> operator*(float num) const{
			point_t<T> ret(x_ * num, y_ * num, z_ * num);
			return ret;	
		}

//		point_t& operator=(const point_t &pnt);
	};
template<typename T>
	class line_t{        // {r : r = root + dir * t , t is real} 
	
		point_t<T> root_, dir_;
	
	public:
	
		line_t();                                     // default line x = y
		line_t(struct point_t<T> a1, struct point_t<T> a2); // constructs the line passing through points a1 and a2
		
		bool valid() const;
		void print() const;								           // prints on screen: "ax + by + c = 0"
		bool perpendicular(line_t<T> const &another) const;
		T angle(point_t<T> const &pnt1, point_t<T> const &pnt2) const;
		
		point_t<T> get_root() const{
			return root_;

		}
		point_t<T> get_dir() const{
			return dir_;
		}
	};
template<typename T>
	struct plane_t {     // ax + by + cz + d = 0
	private:

		T a_, b_, c_, d_;

	public:

		plane_t(T a = 0.0, T b = 0.0, T c = 1.0, T d = 0.0);
		plane_t(point_t<T> p1, point_t<T> p2, point_t<T> p3);
		plane_t(line_t<T> line, point_t<T> pnt);

		point_t<T> get_outer_point();
		point_t<T> intersection(line_t<T> const &line) const;
		enum areas_t get_side_area(point_t<T> const &point) const;         // tells in what area the point is lying
		bool separates(point_t<T> const &pnt1, point_t<T> const &pnt2) const; // checks if the plane separates two points		
		point_t<T> get_normal() const;

		void print() const;

		bool valid() const;

	};

template<typename T>
	struct polygon_t{ // (x1, y1, z1) (x2, y2, z2) ... (xn, yn, zn)
	//private:

		std::vector<point_t<T>> vertices;

//	public:
		polygon_t();                                  // empty polygon
		polygon_t(std::vector<point_t<T>> vertices);     // constructing by vector<point_t> array

		T square() const;                               // returns the square of polygon
		void print() const;                                 // prints: "Polygon is nangle: (x1, y1) (x2, y2) ... (xn, yn)"
		
		line_t<T> get_side(int index) const;                   // return the line holding the polygon side (n side is a line constructed with nth and (n + 1)th vertex of polygon)

		bool valid() const;
		bool is_divided_by_side_plane(polygon_t<T> const & another) const;
		bool intersect(polygon_t<T> const & another) const;                                      // checks if polygon intersects with "another" polygon
		bool holding(point_t<T> const & vert) const;											  // tells if this polygon is holding the vert as one of it's vertices
		void add(point_t<T> const &vert);														  // adds the vert to the tail of vertices if its not in vertices already
	};

};