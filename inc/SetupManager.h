/*
 * Scintillator.h
 *
 *  Created on: Mar 30, 2016
 *      Author: rsehgal
 */

#ifndef INC_SETUPMANAGER_H_
#define INC_SETUPMANAGER_H_

//#include "Component.h"
#include <vector>
#include "Scintillator.h"
#include "RPC.h"
#include "TThread.h"

 namespace Tracking{

    
    class SetupManager{
    private:
        std::vector<ScintillatorPlane*> fScintVector;
        std::vector<RPC*> fRpcVector;
    public:
        SetupManager(){}

        template<typename Type,bool ForRpc>
        void Register(Type *component){
            if(ForRpc)
              fRpcVector.push_back(component);
            else
              fScintVector.push_back(component);
        }

        void RegisterScintillatorPlane(ScintillatorPlane *scintPlane){
            fScintVector.push_back(scintPlane);
        }

        void RegisterRpc(RPC *rpc){
            fRpcVector.push_back(rpc);
        }

        //int GetSize(){return fComponents.size();}

        void GetComponentsName(){
            for(int i = 0 ; i < fScintVector.size() ; i++){
                std::cout<<" Name : " << fScintVector[i]->GetName() << std::endl;
            }
            for(int i = 0 ; i < fRpcVector.size() ; i++){
                std::cout<<" Name : " << fRpcVector[i]->GetName() << std::endl;
            }
        }

        void PrintStrips(){
            for(int i = 0 ; i < fRpcVector.size() ; i++){
                fRpcVector[i]->PrintStrips();
            }
            std::cout<<"Num of Strip in Rpc : " << fRpcVector[0]->GetRpc()->GetNumOfScintillators() << std::endl;
        }



        //-------------------------------------------------

        void *handle(void *ptr){
            int evNo=0;
            Tree t("6147.root","BSC_DATA_TREE");
            while(true){
                sleep(2);
                for(int j = 0 ; j < fScintVector.size() ; j++){

                    Reset(j);
                    std::cout<<"Plane Name : " << fScintVector[j]->GetName() << " :: PlaneSize : "<< fScintVector[j]->GetNumOfScintillators() << std::endl;
                    for(int i = 0 ; i < fScintVector[j]->GetNumOfScintillators() ; i++){
                    fScintVector[j]->GetScintillatorPlane()[i]->DetectAndSetHit<false>(t,evNo); 
                    bool hit =  fScintVector[j]->GetScintillatorPlane()[i]->GetScintHit();
                    std::cout<< " EvNo: " << evNo <<"  :: Strip No : " << i <<"  :: HIT : " << hit <<  std::endl;

                    if(hit)
                        fScintVector[j]->GetScintillatorPlane()[i]->GetScintillatorEveGeoShape()->SetMainColor(2);
                    }
             
                }

                for(int j = 0 ; j < fRpcVector.size() ; j++){
                    ResetRpc(j);
                    std::cout<<"Plane Name : " << fRpcVector[j]->GetName() << " :: PlaneSize : "<< fRpcVector[j]->GetRpc()->GetNumOfScintillators() << std::endl;
                    for(int i = 0 ; i < fRpcVector[j]->GetRpc()->GetNumOfScintillators() ; i++){
                    //fRpcVector[j]->GetScintillatorPlane()[i]->DetectAndSetHit<true>(t,evNo); 
                    fRpcVector[j]->GetRpc()->GetScintillatorPlane()[i]->DetectAndSetHit<true>(t,evNo); 
                    //bool hit =  fRpcVector[j]->GetScintillatorPlane()[i]->GetScintHit();
                    bool hit =  fRpcVector[j]->GetRpc()->GetScintillatorPlane()[i]->GetScintHit();
                    std::cout<< " EvNo: " << evNo <<"  :: Strip No : " << i <<"  :: HIT : " << hit <<  std::endl;

                    if(hit)
                        //fRpcVector[j]->GetScintillatorPlane()[i]->GetScintillatorEveGeoShape()->SetMainColor(2);
                        fRpcVector[j]->GetRpc()->GetScintillatorPlane()[i]->GetScintillatorEveGeoShape()->SetMainColor(2);
                    }
             
                }
                evNo++;
            }
        }

        void Reset(int j){
            
                for(int i = 0 ; i < fScintVector[j]->GetNumOfScintillators() ; i++ ){
                    fScintVector[j]->GetScintillatorPlane()[i]->GetScintillatorEveGeoShape()->SetMainColor(3);
                }
            
        }

        void ResetRpc(int j){
            
                for(int i = 0 ; i < fRpcVector[j]->GetRpc()->GetNumOfScintillators() ; i++ ){
                    fRpcVector[j]->GetRpc()->GetScintillatorPlane()[i]->GetScintillatorEveGeoShape()->SetMainColor(3);
                }
            
        }


        void RunThread(){
  
        TThread* mythread= new TThread("My Thread",
           (void(*) (void *))&SetupManager::handle,
           (void*) this);
        mythread->Run();
        }

    };

}//end of Tracking namespace

#endif
