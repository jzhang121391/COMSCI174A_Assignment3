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

bool DEBUG = 0;
const int MAX_SPHERES = 5;
const int MAX_LIGHTS = 5;

int g_width;
int g_height;
vec4 g_dir;

struct Ray
{
    vec4 origin;
    vec4 dir;
};

// TODO: add structs for spheres, lights and anything else you may need.

struct Color
{
	float r,g,b; // Range 0-1
};

struct Scaling
{
	float x,y,z;
};

struct Sphere
{
	string name;
	vec4 position;
	Scaling scaling;
	Color color;
	float Ka, Kd, Ks, Kr;
	float n;
};

struct Light
{
	string name;
	vec4 position;
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

char g_file[999];

// Arrays to store spheres and lights in case more than 1
Sphere sArray[MAX_SPHERES];
Light lArray[MAX_LIGHTS];

// Counters to traverse array during import and keep count of "active" objects
int sCounter = 0;
int lCounter = 0;

// Initialize objects
Back back;
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
		sArray[sCounter].position.w = 1.0f;

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
		lArray[lCounter].position.w = 1.0f;

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

bool rsIntersect(const Ray& r, const Sphere& s, float &t)
{	
	vec4 dist;
	dist = s.position-r.origin; // Calculate vector from origin to sphere

	mat4 M; // Create matrix for scaling
	M = Scale(s.scaling.x, s.scaling.y, s.scaling.z);
	mat4 invM; // Invert matrix
	InvertMatrix(M, invM);

	vec4 S = invM*dist; // Find S'
	vec4 c = invM*g_dir; // Find c'

	float A = dot(c, c); // Calculate A for quadratic equation
	float B = dot(S, c); // Calculate B " "
	float C = dot(S, S) - 1; // Calculate C " "

	if((B*B - A*C) < 0) return false; // No intersection if behind camera
	
	float t0 = B/A - (sqrtf(B*B - A*C)/A); // Solve quadratic equation
	float t1 = B/A + (sqrtf(B*B - A*C)/A);

	if((t0 > 1.0f) && (t0 < t) && (t0 < t1)) // Find closer t and use it
	{
		t = t0;
		return true; // Intersection exists!
	}
	else if((t1 > 1.0f) && (t1 < t) && (t1 < t0))
	{
		t = t1;
		return true;
	}
	return false;
}

// -------------------------------------------------------------------
// Ray tracing

vec4 trace(Ray viewRay, Sphere* s, Light* l)
{
    // TODO: implement your ray tracing routine here.
	// Background color if ray hits nothing
	vec4 pixelColor = vec4(back.color.r, back.color.g, back.color.b, 0.0f);
	float coef = 1.0f;
	int recursionLevel = 0;
	do
	{
		vec4 hitPoint; // Initialized point of intersection
		int currSphere = -1; // Counter to iterate through sphere container
		{
			float t = 2000.0f; // Arbitrary intersection distance
			for(int i = 0; i < sCounter; i++) // Parse through "active" spheres
				if(rsIntersect(viewRay, sArray[i], t)) // If intersect
				{
					// Set color to sphere's colors
					currSphere = i; // Set current sphere as active sphere
					pixelColor.x = sArray[i].color.r*ambient.intensity.r*sArray[currSphere].Ka;
					pixelColor.y = sArray[i].color.g*ambient.intensity.g*sArray[currSphere].Ka;
					pixelColor.z = sArray[i].color.b*ambient.intensity.b*sArray[currSphere].Ka;
				}
			if(currSphere == -1) // No intersection with sphere found
				break;
			hitPoint = viewRay.origin + t*g_dir; // Calculate coordinate hit
		}

		vec4 N = hitPoint-sArray[currSphere].position;// - sArray[currSphere].position); // Find normal
		mat4 M; // Create matrix for scaling
		M = Scale(sArray[currSphere].scaling.x, sArray[currSphere].scaling.y, sArray[currSphere].scaling.z);
		mat4 invM;
		InvertMatrix(transpose(M), invM); // Find transformed N
		N.w = 0;
		N = normalize(invM*N);

		Ray L; // Create light vector
		L.origin = hitPoint;

		vec4 totalDiff = vec4(0.0f, 0.0f, 0.0f, 0.0f); // Store cumulative diffusion
		vec4 totalSpec = vec4(0.0f, 0.0f, 0.0f, 0.0f); // Store cumulative specular

		for(int j = 0; j < lCounter; j++) // Parse active lights
		{
			Light currLight = lArray[j];

			L.dir = currLight.position-hitPoint;
			vec4 R = 2*N*dot(N, L.dir)-L.dir;

			float fLightProjection = dot(L.dir, N);

			if(fLightProjection <= 0.0f)
				continue;

			float lightDist = dot(L.dir,L.dir); // Calculate light ray distance
			{
				float temp = lightDist;
				if(temp == 0.0f)
					continue;
				temp = 1/sqrtf(temp);
				L.dir = temp*L.dir;
				fLightProjection = temp*fLightProjection;
			}

			bool blocked = false;
			{
				float t = lightDist;
				for(int i = 0; i < sCounter; i++) // Parse active spheres
					if(rsIntersect(L, sArray[i], t)) // Reached by light ray?
					{
						blocked = true;
						break;
					}
			}

			if(!blocked) // If light ray not blocked
			{
				// Calculate diffusion from all source and sum into totalDiff
				float diff = dot(L.dir,N)*coef;
				totalDiff.x += diff*currLight.intensity.r*sArray[currSphere].color.r;
				totalDiff.y += diff*currLight.intensity.g*sArray[currSphere].color.g;
				totalDiff.z += diff*currLight.intensity.b*sArray[currSphere].color.b;
			
				float fViewProjection = dot(viewRay.dir,N);
				vec4 blinnDir = L.dir-viewRay.dir;
				float temp = dot(blinnDir,blinnDir);
				if(temp != 0.0f)
				{
					// Calculate specular from all source and sum into totalSpec
					float spec = 1/sqrtf(temp)*max(fLightProjection-fViewProjection, 0.0f);
					spec = powf(spec, sArray[currSphere].n)*coef;
					totalSpec.x += spec*currLight.intensity.r;
					totalSpec.y += spec*currLight.intensity.g;
					totalSpec.z += spec*currLight.intensity.b;
				}
			}
		}

		// Multiply cumulative diffuse and specular effects with respective components
		// Add to color to be returned
		pixelColor.x += totalDiff.x*sArray[currSphere].Kd + totalSpec.x*sArray[currSphere].Ks;
		pixelColor.y += totalDiff.y*sArray[currSphere].Kd + totalSpec.y*sArray[currSphere].Ks;
		pixelColor.z += totalDiff.z*sArray[currSphere].Kd + totalSpec.z*sArray[currSphere].Ks;

		// Calculate reflection (NOT WORKING PROPERLY)
		coef *= sArray[currSphere].Kr;
		float reflet = 2.0f * dot(viewRay.dir,N);
		viewRay.origin = hitPoint;
		viewRay.dir = viewRay.dir - reflet * N;
		recursionLevel++;
	} while ((coef > 0.0f) && (recursionLevel < 4));
	return pixelColor;
}

float calcX(float i)
{
	return g_left+(i/g_width)*(g_right-g_left); // Calculate pixel on plane's x
}

float calcY(float j)
{
	return g_bottom+(j/g_height)*(g_top-g_bottom); // Calculate pixel on plane's y
}

vec4 getDir(int ix, int iy)
{
    // TODO: modify this. This should return the direction from the origin
    // to pixel (ix, iy), normalized.
    vec4 dir;
    dir = vec4(calcX(ix), calcY(iy), -g_near, 0.0f);
	g_dir = dir;
    return normalize(dir);
}

void renderPixel(int ix, int iy)
{
    Ray ray;
    ray.origin = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    ray.dir = getDir(ix, iy);
    vec4 color = trace(ray, sArray, lArray);
	//vec4 color = vec4(1,1,0.5,1);
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
				if((((float*)g_colors[y*g_width+x])[i] * 255.9f) < 255)
					buf[y*g_width*3+x*3+i] = (unsigned char)(((float*)g_colors[y*g_width+x])[i] * 255.9f);
				else
					buf[y*g_width*3+x*3+i] = (unsigned char)((float*)255); // Clamp max to 255 if over
    
    // TODO: change file name based on input file name.
    savePPM(g_width, g_height, g_file, buf); 
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

	strcpy(g_file, argv[1]);
	for(int i = 0; i < 9999; i++)
		if(g_file[i] == 't' && g_file[i+1] == 'x' && g_file[i+2] == 't')
		{
			g_file[i] = 'p';
			g_file[i+1] = 'p';
			g_file[i+2] = 'm';
		}
    loadFile(argv[1]);

	if(DEBUG == 1)
		debug();

    render();
    saveFile();
	return 0;
}