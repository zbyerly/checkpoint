// Adrian Serio
// April 28, 2017
//
// This example test the checkpooint function.
// Checkpoint is designed to serialize an object and save it as a
// byte stream. Resurrect converts the byte stream back into the 
// object.
// To-Do:
//    - Create a type Checkpoint to manage data stream
//    - Wrap serialized data in a component?
//       -> Return a GID
//    - Add tests
//    - Create as a header file

#include <hpx/hpx_main.hpp>
#include <hpx/include/serialization.hpp>
#include <file.hpp>

template <typename ...T>
std::vector<char>  checkpoint (T const& ...t) {
 std::vector<char> v;
 {
  hpx::serialization::output_archive ar(v);
  int const sequencer[]= {  //Trick to expand the variable pack
   (ar<<t, 0)...};          //Takes advantage of the comma operator
 }
 return v;
}

template <typename ...T>
void resurrect (std::vector<char> v, T& ...t) {
 {
  hpx::serialization::input_archive ar(v, v.size());
  int const sequencer[]= {  //Trick to exand the variable pack
   (ar >> t, 0)...};        //Takes advantage of the comma operator
 } 
}

//template <typename C, typename ...T>
//void resurrect (C& c, T& ...t) {
// {
//  hpx::serialization::input_archive ar(c, c.size());
//  int const sequencer[]= {  //Trick to exand the variable pack
//   (ar >> t, 0)...};        //Takes advantage of the comma operator
// } 
//}
//
//struct Checkpoint {
// std::vector<char> data;
// int size() { return data.size();}
// Checkpoint operator=(std::vector<char> v) {
//  data = std::move(v);
//  return *this;
// }
// char operator[](int i) { return data[i]; }
//};

int main() {
 
 char character='d';
 char character2;
 int integer=10;
 int integer2;
 float flt=10.01;
 float flt2;
 bool boolean=true; 
 bool boolean2; 
 std::string str="I am a string of characters";
 std::string str2;
 std::vector<char> vec(str.begin(),str.end());
 std::vector<char> vec2;
 std::vector<char> archive;
 std::vector<char> archive2;
// Checkpoint archive2;
 
 //Test 1
 hpx::cout<<"Test 1:"<<std::endl;

 //Print out initial variables
 hpx::cout<<"Initial Variables"<<std::endl
          <<" char= "<<character<<std::endl
          <<" int= "<<integer<<std::endl
          <<" float= "<<flt<<std::endl
          <<" bool= "<<boolean<<std::endl
          <<" string= "<<str<<std::endl
          <<" vector<char>= ";
 for (int i=0; i<vec.size(); i++) {
  if (vec.size() != i+1 ) hpx::cout<<vec[i];
  else hpx::cout<<vec[i]<<std::endl;
 }
 
 archive=checkpoint(character, integer, flt, boolean, str, vec);
 resurrect(archive, character2, integer2, flt2, boolean2, str2, vec2);
 
 //Print out resurected variables 
 hpx::cout<<"Resurrected Variables"<<std::endl
          <<" char= "<<character2<<std::endl
          <<" int= "<<integer2<<std::endl
          <<" float= "<<flt2<<std::endl
          <<" bool= "<<boolean2<<std::endl
          <<" string= "<<str2<<std::endl
          <<" vector<char>= ";
 for (int i=0; i<vec2.size(); i++) {
  if (vec2.size() != i+1 ) hpx::cout<<vec2[i];
  else hpx::cout<<vec2[i]<<std::endl;
 }

 if (character == character2 && integer == integer2 && flt == flt2
      && boolean == boolean2 && str == str2 
      && std::equal(vec.begin(), vec.end(), vec2.begin())){
  hpx::cout<<"I work!"<<std::endl;
 }

 //Test 2
 hpx::cout<<"Test 2:"<<std::endl;
 archive2=checkpoint(vec, integer);
 resurrect(archive2, vec2, integer2);
 
 if (std::equal(vec.begin(), vec.end(), vec2.begin()) && integer==integer2){
  hpx::cout<<"I work!"<<std::endl;
 }
 return 0;
}
