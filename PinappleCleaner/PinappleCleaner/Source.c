#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define NoProperties	2
#define NoDimensions	3
#define NoParticles	50
#define NoFrames	8000

#define StrengthOGravity 100
#define Step	0.02
#define Startx	0.510295;

double x = Startx;

void AddTwoParticle1(double *particleset)
{
	particleset[FourToOneInt(0, 0, 0, 0)] = 10000;
	particleset[FourToOneInt(0, 0, 0, 1)] = 9800;
	particleset[FourToOneInt(1, 0, 1, 0)] = -0.4;
	particleset[FourToOneInt(1, 0, 1, 1)] = +0.4;
}

void AddRandomness(double *particleset, double PosRand, double VelRand)
{
	for (int p = 0; p < NoProperties; p++)
	{
		for (int d = 0; d < NoDimensions; d++)
		{
			for (int i = 0; i < NoParticles; i++)
			{
				particleset[FourToOneInt(0, 0, d, i)] += x * PosRand;
				x = (2 * (x)*(x)) - 1;
			}
		}
		for (int d = 0; d < NoDimensions; d++)
		{
			for (int i = 0; i < NoParticles; i++)
			{
				particleset[FourToOneInt(1, 0, d, i)] += x * VelRand;
				x = (2 * (x)*(x)) - 1;
			}
		}
	}
}

void WriteToFile(double *particleset, char FileLocation[])
{
	FILE *fp;
	fp = fopen(FileLocation, "w");
	fprintf(fp, "%i", NoProperties);
	fprintf(fp, ",");
	fprintf(fp, "%i", NoFrames);
	fprintf(fp, ",");
	fprintf(fp, "%i", NoDimensions);
	fprintf(fp, ",");
	fprintf(fp, "%i", NoParticles);
	fprintf(fp, "\n");

	double MaxRange = 0;

	for (int b = 0; b < NoFrames; b++)
	{

		for (int a = 0; a < NoProperties; a++)
		{
			for (int d = 0; d < NoParticles; d++)
			{
				for (int c = 0; c < NoDimensions; c++)
				{
					fprintf(fp, "%g", particleset[FourToOneInt(a, b, c, d)]);
					fprintf(fp, ",");
				}
			}
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}

int FourToOneInt(int Prop, int Frames, int Dim, int Part)
{
	int ret = 0;
	ret += (Part);
	ret += (Dim*NoParticles);
	ret += (Frames*NoDimensions*NoParticles);
	ret += (Prop*NoFrames*NoDimensions*NoParticles);
	return ret;
}

int main()
{
	double *particles = malloc(sizeof(double)*NoProperties*NoFrames*NoDimensions*NoParticles);
	for (int b = 0; b < NoFrames; b++)
	{
		for (int d = 0; d < NoParticles; d++)
		{
			for (int a = 0; a < NoProperties; a++)
			{
				for (int c = 0; c < NoDimensions; c++)
				{
					particles[FourToOneInt(a, b, c, d)] = 0;
				}
			}
		}
	}

	//AddTwoParticle1(particles);
	AddRandomness(particles, 1, 0.04);

	for (int t = 0; t < NoFrames - 1; t++)
	{
		for (int i = 0; i < NoParticles; i++)
		{
			for (int j = 0; j < NoParticles; j++)
			{
				if (i != j)
				{
					double diff = 0;
					double distsquared = 0;
					double maga[NoDimensions];
					double magg[NoDimensions];
					for (int d = 0; d < NoDimensions; d++)
					{
						diff = particles[FourToOneInt(0, t, d, j)] - particles[FourToOneInt(0, t, d, i)];
						maga[d] = magg[d] = diff;
						distsquared += diff*diff;
					}
					double dist = sqrt(distsquared);
					for (int d = 0; d < NoDimensions; d++)
					{
						maga[d] /= (pow(dist, 4) / StrengthOGravity);
						magg[d] /= (pow(dist, 3)/StrengthOGravity);
						particles[FourToOneInt(1, t + 1, d, i)] = (particles[FourToOneInt(1, t, d, i)]);// -(Step * ((particles[FourToOneInt(1, t, d, i)]))) / 5000;
						particles[FourToOneInt(1, t + 1, d, i)] = (particles[FourToOneInt(1, t, d, i)]) + (magg[d] * Step) - (maga[d] * Step);
					}
				}

			}
			for (int d = 0; d < NoDimensions; d++)
			{
				particles[FourToOneInt(0, t + 1, d, i)] = particles[FourToOneInt(0, t, d, i)] + particles[FourToOneInt(1, t + 1, d, i)] * Step;
				if ((t % 300) == 0)
				{
					printf("%g", particles[FourToOneInt(0, t, d, i)]);
					printf("   \t");
				}
			}
		}
		if ((t % 300) == 0)
		{
			printf("\n");
		}
	}

	WriteToFile(particles,"C:\\Users\\Admin\\Documents\\text18.txt");

	
}