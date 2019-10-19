#pragma once
#include <vector>
#include <array>
#include <cmath>

namespace lingeo3D{

	extern const float flt_tolerance;                 // tolerance that will be used in calculation of coordinates (e.g. for comparing the points)
	extern float inter_area_width;                    // defines the width (counted in flt_tolerances) of INTER_AREA that used to avoid some corner cases
	enum areas_t {LEFT_SIDE, INTER_SIDE, RIGHT_SIDE}; // there are 3 areas that are defined by the line cut of plane: left, right and inter area. The last one represents the same line with defined thickness (inter_are_width)
	enum degen_t {NORMAL_DEG, LINE_DEG, POINT_DEG, INVALID_DEG};

	struct point_t {      // (x , y, z)
	private:

		float x_, y_, z_;

	public:
		point_t(float x = NAN, float y = NAN, float z = NAN);
		point_t(const point_t &point);
		
		void print() const;		// prints on screen: "(x ; y)"

		bool valid() const;
		point_t vector_prod(point_t const &pnt) const;
		float scalar_prod(point_t const &pnt) const;
		float distance(point_t const &pnt) const;
		bool operator==(point_t const &pnt) const;
		bool operator!=(point_t const &pnt) const;
		point_t operator-(point_t const &another) const;
		point_t operator+(point_t const &another) const;
		point_t operator*(float num) const;

		float get_x() const;
		float get_y() const;
		float get_z() const;
//		point_t& operator=(const point_t &pnt);
	};

	struct line_t{        // {r : r = root + dir * t , t is real} 
	private:
	
		point_t root_, dir_;
	
	public:
	
		line_t();                                     // default line x = y
		line_t(struct point_t a1, struct point_t a2); // constructs the line passing through points a1 and a2
		
		bool valid() const;
		void print() const;								           // prints on screen: "ax + by + c = 0"
		bool perpendicular(line_t const &another) const;
		float angle(point_t const &pnt1, point_t const &pnt2) const;
		
		point_t get_root() const;
		point_t get_dir() const;
	};

	struct plane_t {     // ax + by + cz + d = 0
	private:

		float a_, b_, c_, d_;

	public:

		plane_t(float a = 0.0, float b = 0.0, float c = 1.0, float d = 0.0);
		plane_t(point_t p1, point_t p2, point_t p3);
		plane_t(line_t line, point_t pnt);

		point_t get_outer_point();
		point_t intersection(line_t const &line) const;
		enum areas_t get_side_area(point_t const &point) const;         // tells in what area the point is lying
		bool separates(point_t const &pnt1, point_t const &pnt2) const; // checks if the plane separates two points		
		point_t get_normal() const;

		void print() const;

		bool valid() const;

	};


	struct polygon_t{ // (x1, y1, z1) (x2, y2, z2) ... (xn, yn, zn)
	//private:

		std::vector<point_t> vertices;

	public:
		polygon_t();                                  // empty polygon
		polygon_t(std::vector<point_t> vertices);     // constructing by vector<point_t> array

		float square() const;                               // returns the square of polygon
		void print() const;                                 // prints: "Polygon is nangle: (x1, y1) (x2, y2) ... (xn, yn)"
		
		line_t get_side(int index) const;                   // return the line holding the polygon side (n side is a line constructed with nth and (n + 1)th vertex of polygon)

		bool valid() const;
		bool is_divided_by_side_plane(polygon_t const & another) const;
		bool intersect(polygon_t const & another) const;                                      // checks if polygon intersects with "another" polygon
		bool holding(point_t const & vert) const;											  // tells if this polygon is holding the vert as one of it's vertices
		void add(point_t const &vert);														  // adds the vert to the tail of vertices if its not in vertices already
	};

};