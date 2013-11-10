//
// template-rt.cpp
//

#define _CRT_SECURE_NO_WARNINGS
#include "matm.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

bool DEBUG = 1;
const int MAX_SPHERES = 5;
const int MAX_LIGHTS = 5;

int g_width;
int g_height;

struct Ray
{
    vec4 origin;
    vec4 dir;
};

// TODO: add structs for spheres, lights and anything else you may need.

struct Position
{
	float x,y,z;
};

struct Color
{
	float r,g,b;
};

struct Scaling
{
	float x,y,z;
};

struct Sphere
{
	string name;
	Position position;
	Scaling scaling;
	Color color;
	float Ka, Kd, Ks, Kr;
	float n;
};

struct Light
{
	string name;
	Position position;
	Color intensity;
};

struct Back
{
	Color color;
};

struct Ambient
{
	Color intensity;
};

// Arrays to store spheres and lights in case more than 1
Sphere sArray[MAX_SPHERES];
Light lArray[MAX_LIGHTS];

// Counters to traverse array as program imports
int sCounter = 0;
int lCounter = 0;

// Initialize Back
Back back;

// Initialize Ambient
Ambient ambient;

vector<vec4> g_colors;

float g_left;
float g_right;
float g_top;
float g_bottom;
float g_near;


// -------------------------------------------------------------------
// Input file parsing

vec4 toVec4(const string& s1, const string& s2, const string& s3)
{
    stringstream ss(s1 + " " + s2 + " " + s3);
    vec4 result;
    ss >> result.x >> result.y >> result.z;
    result.w = 1.0f;
    return result;
}

float toFloat(const string& s)
{
    stringstream ss(s);
    float f;
    ss >> f;
    return f;
}

void parseLine(const vector<string>& vs)
{
    //TODO: add parsing of NEAR, LEFT, RIGHT, BOTTOM, TOP, SPHERE, LIGHT, BACK, AMBIENT, OUTPUT.
    if (vs[0] == "NEAR")
		g_near = toFloat(vs[1]);
	if (vs[0] == "LEFT")
		g_left = toFloat(vs[1]);
	if (vs[0] == "RIGHT")
		g_right = toFloat(vs[1]);
	if (vs[0] == "BOTTOM")
		g_bottom = toFloat(vs[1]);
	if (vs[0] == "TOP")
		g_top = toFloat(vs[1]);
	if (vs[0] == "RES")
    {
        g_width = (int)toFloat(vs[1]);
        g_height = (int)toFloat(vs[2]);
        g_colors.resize(g_width * g_height);
    }
	if (vs[0] == "SPHERE")
	{
		// Import name
		sArray[sCounter].name = vs[1];

		// Import position
		sArray[sCounter].position.x = toFloat(vs[2]);
		sArray[sCounter].position.y = toFloat(vs[3]);
		sArray[sCounter].position.z = toFloat(vs[4]);

		// Import scaling
		sArray[sCounter].scaling.x = toFloat(vs[5]);
		sArray[sCounter].scaling.y = toFloat(vs[6]);
		sArray[sCounter].scaling.z = toFloat(vs[7]);

		// Import Color
		sArray[sCounter].color.r = toFloat(vs[8]);
		sArray[sCounter].color.g = toFloat(vs[9]);
		sArray[sCounter].color.b = toFloat(vs[10]);

		// Import K
		sArray[sCounter].Ka = toFloat(vs[11]);
		sArray[sCounter].Kd = toFloat(vs[12]);
		sArray[sCounter].Ks = toFloat(vs[13]);
		sArray[sCounter].Kr = toFloat(vs[14]);

		// Import n
		sArray[sCounter].n = toFloat(vs[15]);
		
		// Increment sphere array for new sphere
		sCounter++;
	}
	if (vs[0] == "LIGHT")
	{
		// Import name
		lArray[lCounter].name = vs[1];

		// Import position
		lArray[lCounter].position.x = toFloat(vs[2]);
		lArray[lCounter].position.y = toFloat(vs[3]);
		lArray[lCounter].position.z = toFloat(vs[4]);

		// Import intensity
		lArray[lCounter].intensity.r = toFloat(vs[5]);
		lArray[lCounter].intensity.g = toFloat(vs[6]);
		lArray[lCounter].intensity.b = toFloat(vs[7]);

		// Increment light array for new light
		lCounter++;
	}
	if (vs[0] == "BACK")
	{
		//Import color
		back.color.r = toFloat(vs[1]);
		back.color.g = toFloat(vs[2]);
		back.color.b = toFloat(vs[3]);
	}
	if (vs[0] == "AMBIENT")
	{
		ambient.intensity.r = toFloat(vs[1]);
		ambient.intensity.g = toFloat(vs[2]);
		ambient.intensity.b = toFloat(vs[3]);
	}

}

void loadFile(const char* filename)
{
    ifstream is(filename);
    if (is.fail())
    {
        cout << "Could not open file " << filename << endl;
        exit(1);
    }
    string s;
    vector<string> vs;
    while(!is.eof())
    {
        vs.clear();
        getline(is, s);
        istringstream iss(s);
        while (!iss.eof())
        {
            string sub;
            iss >> sub;
            vs.push_back(sub);
        }
        parseLine(vs);
    }
}


// -------------------------------------------------------------------
// Utilities

void setColor(int ix, int iy, const vec4& color)
{
    int iy2 = g_height - iy - 1; // Invert iy coordinate.
    g_colors[iy2 * g_width + ix] = color;
}


// -------------------------------------------------------------------
// Intersection routine

// TODO: add your ray-sphere intersection routine here.

/*float rsIntersect(const Sphere* sphere, const Ray* ray)
{
	vec4 v;
	v = sub(sphere->position, ray->origin);
	float b = dot(ray->dir, v);
	float disc = b*b - dot(v,v) + ...;

	if(disc < 0.0)
		return fInfinity;
	float d = sqrt(disc);
	float t1 = b-d;
	float t2 = b+d;
	if(t2 < 0.0)
		return fInfinity;
	else
		return (t1 > 0.0 ? t1 : t2);
}*/

// -------------------------------------------------------------------
// Ray tracing

vec4 trace(const Ray& ray)
{
    // TODO: implement your ray tracing routine here.
    return vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

vec4 getDir(int ix, int iy)
{
    // TODO: modify this. This should return the direction from the origin
    // to pixel (ix, iy), normalized.
    vec4 dir;
    dir = vec4(0.0f, 0.0f, -1.0f, 0.0f);
    return dir;
}

void renderPixel(int ix, int iy)
{
    Ray ray;
    ray.origin = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    ray.dir = getDir(ix, iy);
    vec4 color = trace(ray);
    setColor(ix, iy, color);
}

void render()
{
    for (int iy = 0; iy < g_height; iy++)
        for (int ix = 0; ix < g_width; ix++)
            renderPixel(ix, iy);
}


// -------------------------------------------------------------------
// PPM saving

void savePPM(int Width, int Height, char* fname, unsigned char* pixels) 
{
    FILE *fp;
    const int maxVal=255;

    printf("Saving image %s: %d x %d\n", fname, Width, Height);
    fp = fopen(fname,"wb");
    if (!fp) {
        printf("Unable to open file '%s'\n", fname);
        return;
    }
    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n", Width, Height);
    fprintf(fp, "%d\n", maxVal);

    for(int j = 0; j < Height; j++) {
        fwrite(&pixels[j*Width*3], 3, Width, fp);
    }

    fclose(fp);
}

void saveFile()
{
    // Convert color components from floats to unsigned chars.
    // TODO: clamp values if out of range.
    unsigned char* buf = new unsigned char[g_width * g_height * 3];
    for (int y = 0; y < g_height; y++)
        for (int x = 0; x < g_width; x++)
            for (int i = 0; i < 3; i++)
                buf[y*g_width*3+x*3+i] = (unsigned char)(((float*)g_colors[y*g_width+x])[i] * 255.9f);
    
    // TODO: change file name based on input file name.
    savePPM(g_width, g_height, "output.ppm", buf);
    delete[] buf;
}

void debug()
{
	cout << "NEAR " << g_near << endl;
	cout << "LEFT " << g_left << endl;
	cout << "RIGHT " << g_right << endl;
	cout << "BOTTOM " << g_bottom << endl;
	cout << "TOP " << g_top << endl;
	cout << "RES " << g_width << " " << g_height << endl;
	for(int i = 0; i < MAX_SPHERES; i++)
		if(sArray[i].name != "EMPTY")
		{
			cout << "SPHERE " << sArray[i].name << " " << sArray[i].position.x
				<< " " << sArray[i].position.y << " " << sArray[i].position.z 
				<< " " << sArray[i].scaling.x << " " << sArray[i].scaling.y
				<< " " << sArray[i].scaling.z << " " << sArray[i].color.r
				<< " " << sArray[i].color.g << " " << sArray[i].color.b
				<< " " << sArray[i].Ka << " " << sArray[i].Kd << " "
				<< sArray[i].Ks << " " << sArray[i].Kr << " " << sArray[i].n
				<< endl;
		}
	for(int i = 0; i < MAX_LIGHTS; i++)
		if(lArray[i].name != "EMPTY")
		{
			cout << "LIGHT " << lArray[i].name << " " << lArray[i].position.x
				<< " " << lArray[i].position.y << " " << lArray[i].position.z
				<< " " << lArray[i].intensity.r << " " << lArray[i].intensity.g
				<< " " << lArray[i].intensity.b << endl;
		}
	cout << "BACK " << back.color.r << " " << back.color.g << " " << back.color.b 
		<< endl;
	cout << "AMBIENT " << ambient.intensity.r << " "  << ambient.intensity.g << " " 
		<< ambient.intensity.b << endl;
}


// -------------------------------------------------------------------
// Main

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cout << "Usage: template-rt <input_file.txt>" << endl;
        exit(1);
    }

	// Set indicators of partition not used
	for(int i = 0; i < MAX_SPHERES; i++)
		sArray[i].name = "EMPTY";
	for(int i = 0; i < MAX_LIGHTS; i++)
		lArray[i].name = "EMPTY";

    loadFile(argv[1]);

	if(DEBUG == 1)
		debug();

    render();
    saveFile();
	return 0;
}