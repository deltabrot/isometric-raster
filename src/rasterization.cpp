#include <iostream>
#include <cmath>
#include <SDL.h>
#include "rasterization.h"

Edge::Edge(const Color &color1, int x1, int y1, const Color &color2, int x2, int y2){
	if(y1 < y2){
		Color1 = color1;
		X1 = x1; 
                Y1 = y1; 
                Color2 = color2;
                X2 = x2; 
                Y2 = y2; 
        } else {
                Color1 = color2;
                X1 = x2; 
                Y1 = y2; 
                Color2 = color1;
                X2 = x1; 
                Y2 = y1; 
	}
}

Span::Span(const Color &color1, int x1, const Color &color2, int x2){
        if(x1 < x2) {
                Color1 = color1;
                X1 = x1;
                Color2 = color2;
                X2 = x2;
        } else {
                Color1 = color2;
                X1 = x2;
                Color2 = color1;
                X2 = x1;
        }
}

void Rasterization::setRenderer(SDL_Renderer * renderer){
	m_Renderer = renderer;
}

void Rasterization::drawSpan(const Span &span, int y){
        int xdiff = span.X2 - span.X1;
        if(xdiff == 0)
		return;
        
        Color colordiff = span.Color2 - span.Color1;
        
        float factor = 0.0f;
        float factorStep = 1.0f / (float)xdiff;
        
        // draw each pixel in the span
        for(int x = span.X1; x < span.X2; x++) {
		Color currentColor = span.Color1 + (colordiff * factor);
		SDL_SetRenderDrawColor(m_Renderer, currentColor.R*255, currentColor.G*255, currentColor.B*255, 255 );
		SDL_RenderDrawPoint(m_Renderer, x, y);
                factor += factorStep;
        }
}

void Rasterization::drawSpansBetweenEdges(const Edge &e1, const Edge &e2){
        // calculate difference between the y coordinates
        // of the first edge and return if 0
        float e1ydiff = (float)(e1.Y2 - e1.Y1);
        if(e1ydiff == 0.0f)
                return;
        
        // calculate difference between the y coordinates
        // of the second edge and return if 0
        float e2ydiff = (float)(e2.Y2 - e2.Y1);
        if(e2ydiff == 0.0f)
                return;
        
        // calculate differences between the x coordinates
        // and colors of the points of the edges
        float e1xdiff = (float)(e1.X2 - e1.X1);
        float e2xdiff = (float)(e2.X2 - e2.X1);
        Color e1colordiff = (e1.Color2 - e1.Color1);
        Color e2colordiff = (e2.Color2 - e2.Color1);
        
        // calculate factors to use for interpolation
        // with the edges and the step values to increase
        // them by after drawing each span
        float factor1 = (float)(e2.Y1 - e1.Y1) / e1ydiff;
        float factorStep1 = 1.0f / e1ydiff;
        float factor2 = 0.0f;
        float factorStep2 = 1.0f / e2ydiff;

        // loop through the lines between the edges and draw spans
        for(int y = e2.Y1; y < e2.Y2; y++) {
                // create and draw span
                Span span(e1.Color1 + (e1colordiff * factor1),
                          e1.X1 + (int)(e1xdiff * factor1),
                          e2.Color1 + (e2colordiff * factor2),
                          e2.X1 + (int)(e2xdiff * factor2));
                drawSpan(span, y);

                // increase factors
                factor1 += factorStep1;
                factor2 += factorStep2;
        }
}

void Rasterization::drawTriangle(const Color &color1, float x1, float y1, float x2, float y2, float x3, float y3){
	SDL_SetRenderDrawColor(m_Renderer, color1.R*255, color1.G*255, color1.B*255, 255);
	SDL_RenderDrawLine(m_Renderer, x1, y1, x2, y2);
	SDL_RenderDrawLine(m_Renderer, x2, y2, x3, y3);
	SDL_RenderDrawLine(m_Renderer, x3, y3, x1, y1);
}

void Rasterization::fillTriangle(const Color &color1, float x1, float y1, const Color &color2, float x2, float y2, const Color &color3, float x3, float y3){
        // create edges for the triangle
        Edge edges[3] = {
                Edge(color1, (int)x1, (int)y1, color2, (int)x2, (int)y2),
                Edge(color2, (int)x2, (int)y2, color3, (int)x3, (int)y3),
                Edge(color3, (int)x3, (int)y3, color1, (int)x1, (int)y1)
        };

        int maxLength = 0;
        int longEdge = 0;

        // find edge with the greatest length in the y axis
        for(int i = 0; i < 3; i++) {
                int length = edges[i].Y2 - edges[i].Y1;
                if(length > maxLength) {
                        maxLength = length;
                        longEdge = i;
                }
        }

        int shortEdge1 = (longEdge + 1) % 3;
        int shortEdge2 = (longEdge + 2) % 3;

        // draw spans between edges; the long edge can be drawn
        // with the shorter edges to draw the full triangle
        drawSpansBetweenEdges(edges[longEdge], edges[shortEdge1]);
        drawSpansBetweenEdges(edges[longEdge], edges[shortEdge2]);
}
