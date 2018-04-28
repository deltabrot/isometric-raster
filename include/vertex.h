#ifndef __VERTEX_H__
#define __VERTEX_H__

class Vertex{
	public:
		float x, y, z;
		Vertex(float x, float y, float z);
		Vertex();
		void print();
		Vertex operator + (const Vertex &v) const;
		Vertex operator - (const Vertex &v) const;
		float operator * (const Vertex &v) const;
};

#endif /* __VERTEX_H__ */
