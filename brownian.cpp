#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include "simple_svg_1.0.0.hpp"
#include <string>

const double PI  = 3.141592653589793238463;

using namespace std;
using namespace svg;

double uniformRandom()
{
  	return ( (double)(rand()) + 1. )/( (double)(RAND_MAX) + 1. );
}

double normalRandom(double mu, double sigma) //Box-Muller metoda
{
	double u1 = uniformRandom();
	double u2 = uniformRandom();
	double z0;
	
	z0 = sqrt(-2.0 * log(u1)) * cos(2*PI * u2);
	return z0 * sigma + mu; 
}

int generator(double *cas, int pocetDielikov, double *pole) {

	pole[0]=0;
	
	for(int i = 1; i <= pocetDielikov; i++) {
	pole[i] = pole[i-1] + normalRandom(0,cas[i]-cas[i-1]);
	}
	
	return 0;
}

void ulozenieTrajektorii(double **pole, int N, double dt, int pocetDielikov, double *cas) {
	int  pocitadlo = 0;

	Dimensions dimensions(2000, 500);
    Document doc("trajektorie_"+to_string(N)+".svg", Layout(dimensions, Layout::BottomLeft));
	Document doc2("vektory_"+to_string(N)+".svg", Layout(dimensions, Layout::BottomLeft));
	Polyline trajektoria(Stroke(.5, Color::Aqua));
	Polyline os(Stroke(1.5,Color::Yellow));

	for(int j = 0; j < N; j++) {
		generator(cas,pocetDielikov,pole[j]);
		
		if((pole[j][1] < 0) &&(pole[j][2] > (sqrt(3)+1)*pole[j][1])) {
			pocitadlo++;
			trajektoria = Stroke(0.5,Color::Red);
			doc2 << Circle(Point(100*pole[j][1]+250, 100*pole[j][2]+250), 3, Fill(Color(100, 200, 120)), Stroke(1, Color(200, 250, 150)));
		} else {
			trajektoria = Stroke(0.5,Color::Black);
			doc2 << Circle(Point(100*pole[j][1]+250, 100*pole[j][2]+250), 3, Fill(Color(0,0,0)), Stroke(1, Color(0,0,0)));
		}
		for(int i = 0; i <= pocetDielikov; i++) {
			trajektoria << Point(100 *cas[i],100*pole[j][i]+250);	
		}
	    doc << trajektoria;	 
	}
	
	for(int i = 0; i <= pocetDielikov; i++) {
		os << Point(100 *cas[i],250) << Point(100 *cas[i],275)<<Point(100 *cas[i],225)<<Point(100 *cas[i],250);			
	}
	doc<<os;
	
    cout<<"pravdepodobnost "<<pocitadlo / (double) N<<endl;

    doc.save();
    doc2.save();
}

int main() {
	srand(time(NULL));
	
	int T1, T2, pocetDielikov;
	double dt, **pole, *cas;
	int pocetTrajektorii[3] = {100, 1000, 10000};
	
	//zadanie casovej diskretizacie	
	T1 = 0;
	T2 = 10;
	pocetDielikov = 20;
	dt = (double)T2 / pocetDielikov;
	
	cas = new double [pocetDielikov + 1];
	for(int i = 0; i <= pocetDielikov; i++) {
		cas[i] = T1 + i*dt;
	}
	
	for(int i = 0; i < 3; i++) {
		pole = new double * [pocetTrajektorii[i]];
		for(int j = 0; j < pocetTrajektorii[i]; j++) pole[j] = new double[pocetDielikov + 1];
		
		ulozenieTrajektorii(pole,pocetTrajektorii[i],dt,pocetDielikov,cas);
		
		for(int j = 0; j <= pocetDielikov; j++) delete[] pole[j];
    	delete[] pole;
	}
		
    delete[] cas;
}
