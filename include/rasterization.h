#ifndef __RASTERIZATION_H__
#define __RASTERIZATION_H__

#include <SDL.h>
#include "color.h"

class Edge{
	public:
		Color Color1, Color2;
		int X1, Y1, X2, Y2;
		Edge(const Color &color1, int x1, int y1, const Color &color2, int x2, int y2);
};

class Span{
	public:
		Color Color1, Color2;
		int X1, X2;
		Span(const Color &color1, int x1, const Color &color2, int x2);
};

class Rasterization{
        protected:
                void drawSpan(const Span &span, int y);
                void drawSpansBetweenEdges(const Edge &e1, const Edge &e2);

        public:
		SDL_Renderer* m_Renderer;
                void drawTriangle(const Color &color1, float x1, float y1, float x2, float y2, float x3, float y3);
                void fillTriangle(const Color &color1, float x1, float y1, const Color &color2, float x2, float y2, const Color &color3, float x3, float y3);
		void setRenderer(SDL_Renderer*);
};

#endif /* __RASTERIZATION_H__ */
