#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define NoProperties	3
#define NoDimensions	4
#define NoParticles	2
#define NoFrames	150000
#define StrengthOGravity 100
int main()
{
	double *particles = malloc(sizeof(double)*NoProperties*NoFrames*NoDimensions*NoParticles);
	double x = 0.5015252295235589;
	double Step = 0.03;

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

	particles[FourToOneInt(0, 0, 0, 0)] = 100;
	particles[FourToOneInt(0, 0, 0, 1)] = -100;
	particles[FourToOneInt(1, 0, 1, 0)] = -0.4;
	particles[FourToOneInt(1, 0, 1, 1)] = +0.4;

	for (int p = 0; p < NoProperties; p++)
	{
		for (int d = 0; d < NoDimensions; d++)
		{
			for (int i = 0; i < NoParticles; i++)
			{
				particles[FourToOneInt(0, 0, d, i)] += x * 1;
				x = (2 * (x)*(x)) - 1;
			}
		}
		for (int d = 0; d < NoDimensions; d++)
		{
			for (int i = 0; i < NoParticles; i++)
			{
				particles[FourToOneInt(1, 0, d, i)] += x * 0.004;
				x = (2 * (x)*(x)) - 1;
			}
		}
	}

	double MinDist = 9000;
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
					double mag[NoDimensions];
					for (int d = 0; d < NoDimensions; d++)
					{
						diff = particles[FourToOneInt(0, t, d, j)] - particles[FourToOneInt(0, t, d, i)];
						mag[d] = diff*StrengthOGravity;
						distsquared += diff*diff;
					}
					for (int d = 0; d < NoDimensions; d++)
					{
						mag[d] /= pow(sqrt(distsquared), 3);
						particles[FourToOneInt(1, t + 1, d, i)] = (particles[FourToOneInt(1, t, d, i)]);// -(Step * ((particles[FourToOneInt(1, t, d, i)]))) / 5000;
						particles[FourToOneInt(1, t + 1, d, i)] = (particles[FourToOneInt(1, t, d, i)]) + (mag[d] * Step);
					}
					if (distsquared < MinDist*MinDist)
					{
						MinDist = sqrt(distsquared);
						//printf("%G.4", MinDist);
						//printf("\n");
					}
				}

			}
			for (int d = 0; d < NoDimensions; d++)
			{
				particles[FourToOneInt(0, t + 1, d, i)] = particles[FourToOneInt(0, t, d, i)] + particles[FourToOneInt(1, t + 1, d, i)] * Step;
				//printf("  ");
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
	printf("%G", MinDist);
	FILE *fp;
	fp = fopen("C:\\Users\\Admin\\Documents\\test11.txt", "w");
	//fprintf(fp, "Testing...\n");
	//fprintf(fp, ",");
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
					fprintf(fp, "%g", particles[FourToOneInt(a, b, c, d)]);
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