#include "TEveStraightLineSet.h"
#include "TRandom.h"
#include "Eve/Singleton.h"
#include "TEveManager.h"
#include "TApplication.h"
#include "Eve/EveVisualizer.h"
#include "TTimer.h"

int main(){
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	TEveManager::Create();
	TEveStraightLineSet *ls = new TEveStraightLineSet();
	ls->AddLine(2,3,10,
			    4,5,-100);
	Tracking::Singleton::instance()->AddLineSet(ls);

	//gEve->AddGlobalElement(Tracking::Singleton::instance()->GetLineSet());
	//gEve->DoRedraw3D();


	Tracking::EveVisualizer v;
	v.Show();


	 TTimer timer(20); // every 20 millisecond
	  //// timer.SetCommand("gEve->Redraw3D(kTRUE);");
	  timer.SetCommand("gEve->DoRedraw3D();");
	  timer.TurnOn();
	  fApp->Run();
	fApp->Run();
}
