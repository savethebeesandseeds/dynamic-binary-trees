// -*- coding: utf-16 -*-
// --- --- --- --- --- --- --- --- --- --- --- --- ---
// author   : waajacu
// license  : open-public-free-to-use
// --- --- --- --- --- --- --- --- --- --- --- --- ---
// README   : Utility for defining binary trees
//          : designed for a dyanamical transition tree
//          : desing  :
//                    : there are Nodes
//                                : the color 
//                                : the reference
//                                : the action
//                                : the transiton_function
//          : suports :
//          :         : yields the color of a bool state
//          :         : yields the vector<color> of a vector<bool> sequence
//          :         : yields the node_reference of a bool state
//          :         : yields the vector<node_reference> of a vector<bool> sequence
//          :         : 
//          :         : 
// --- --- --- --- --- --- --- --- --- --- --- --- ---
// FIXES    : 
//          : node validation always returns true
//          : node destructor does not delocate ref vectors
//          : add node action tensor
// --- --- --- --- --- --- --- --- --- --- --- --- ---
// --- --- --- --- --- --- --- --- --- --- --- --- ---
// REFERENCE: 
//          : KaoShi     /    exam
//          : JanSe      /    color
//          : Jiyi       /    memory
//          : CanKao     /    node reference
//          : JieDian_t  /    node
//          : Shu        /    tree
// --- --- --- --- --- --- --- --- --- --- --- --- ---
// EXAMPLE  :
//          : THE EXAMPLE OF GOWI
//          :
//          : Node state transition sums honor points 
//          : Node state transition prints a phrase in the terminal. 
//          :
//          : Honor is the performance mesure of Gowi
//          : Phrases are the narration of Gowi's life
//          : 
//          : Gowi is born with some honor points.
//          : every success gives some honor points.
//          : every failure takes some honor points.
//          : 
//          : Gowi is up to win the most points if 
//          : revolution event is a success.---like a game 
//          : modeleded using dynamic binary nodes.
//          : 
// --- --- --- --- --- --- --- --- --- --- --- --- ---
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 
#include <assert.h>
#include <climits>
#include <vector>
#include <memory>
#include <atomic>
#include <cstring>
#include <list>
#include <iostream>
// --- --- --- --- --- --- --- --- --- --- --- --- --- 
namespace KAOSHI 
{
// --- --- --- --- --- --- --- --- --- --- --- --- --- 
inline std::atomic<unsigned long> global_refenrence_counter=0x00;
// --- --- --- --- --- --- --- --- --- --- --- --- ---
bool node_validation(){return true;} // all nodes are valid #FIXME
// --- --- --- --- --- --- --- --- --- --- --- --- --- 
typedef struct YanSe {
  float honor;
  std::string phrase;
} YanSe_t;
// --- --- --- --- --- --- --- --- --- --- --- --- --- 
typedef struct CanKao { // node reference
  unsigned long unique_reference;
  std::string name;
  void *node_positive;
  void *node_negative;
} CanKao_t;
// --- --- --- --- --- --- --- --- --- --- --- --- --- 
template<typename T>
struct Jiyi_t {         // node load/save object
  std::string name;
  std::string positive_name;
  std::string negative_name;
  T color;
};
// --- --- --- --- --- --- --- --- --- --- --- --- ---
typedef void (*action_function_pointer_t)(YanSe_t*,YanSe_t*);
typedef bool (*transition_boolean_function_pointer_t)(YanSe_t*,bool);
// --- --- --- --- --- --- --- --- --- --- --- --- ---
template <typename T>
class JieDian_t { // node
// --- a node has : --- --- method input --- ---  method output --- 
// --- color      :     ()                      ->  <T>* :: integer, string, ...
// --- action     :     ()                      ->  void
// --- step_transition: (&args)                 ->  bool
// --- self_reference:  (bool)                  ->  CanKao_t*
  public:
    T color;
    CanKao_t *self_reference;
    action_function_pointer_t step_action;
    transition_boolean_function_pointer_t step_transition;
  public:
    JieDian_t(                             // class constructor arguments
      std::string _name, 
      T _color,
      action_function_pointer_t _action,
      transition_boolean_function_pointer_t _transition,
      CanKao_t *_positive_reference,
      CanKao_t *_negative_reference):        // class variables initialization
        color(_color),
        self_reference(new CanKao_t({
          global_refenrence_counter.load(),  
          _name, 
          _positive_reference, 
          _negative_reference
        })),
        step_action(_action),
        step_transition(_transition) 
        {
        }
    virtual ~JieDian_t(){} // #FIXME destructor must inform Shu to update memory vectors
    // --- --- --- --- --- --- --- make action
    template<typename... Args>
    void yield_action(KAOSHI::YanSe_t *payload_color, Args&&... args){
      step_action(payload_color,&color,args...);
    }
    // --- --- --- --- --- --- --- compute transition function
    template<typename... Args>
    CanKao_t *yield_transition(Args&&... args){
      return step_transition(&color,args...) ? \
        ((CanKao_t*)self_reference->node_positive) : ((CanKao_t*)self_reference->node_negative);
    }
    // --- --- --- --- --- --- --- step node
    template<typename... Args>
    CanKao_t* step(Args&&... args){
      return yield_transition(args...);
    }
};
// --- --- --- --- --- --- --- --- --- --- --- --- ---
class Shu { // memory, ---or tree representation
// --- payload_color                : tree state
// --- d_state_ref                  : actual node
// --- node_vector                  : vector of nodes
// --- reference_vector      : vector of node references
// --- note this is a static class 
  public:
    YanSe_t payload_color;
    CanKao_t *d_state_ref;
    std::vector<JieDian_t<YanSe_t>*> node_vector;
    std::vector<CanKao_t*> reference_vector;
  public:
    Shu():
      payload_color({0x00, "\n"}),
      d_state_ref(NULL),
      node_vector(std::vector<JieDian_t<YanSe_t>*>()),
      reference_vector(std::vector<CanKao_t*>())
      {}
    virtual ~Shu(){}
    // --- --- --- --- --- --- --- 
    void set_current_node(CanKao_t *d_ref){
      d_state_ref=d_ref;
      ref_node(d_state_ref)->yield_action(&payload_color);
    }
    void new_node(
      std::string _name, 
      YanSe_t _color,
      action_function_pointer_t _action,
      transition_boolean_function_pointer_t _transition,
      CanKao_t *_positive_reference,
      CanKao_t *_negative_reference){
      assert(node_validation());
      JieDian_t<YanSe_t> *n_node = new JieDian_t<YanSe_t>(
          _name,_color,_action,_transition,_positive_reference,_negative_reference);
      node_vector.push_back(n_node);
      reference_vector.push_back(
        n_node->self_reference);
      global_refenrence_counter++;
    }
    JieDian_t<YanSe_t> *ref_node(CanKao_t *ref){ // gets a node from a node reference
      return ref->unique_reference < global_refenrence_counter.load() ? \
        node_vector.at((int)ref->unique_reference)             // 
        : ref_node(name_reference("END_NODE"));                           // defaults to END_NODE
    }
    CanKao_t *name_reference(std::string name){ // gets a node reference from a node name
      for(CanKao_t *urv : reference_vector){
        if(urv->name.compare(name)==0){return urv;}}
      fprintf(stderr,"[ERROR] reference not found.\n");
      return name_reference("END_NODE");
    }
    JieDian_t<YanSe_t> *name_node(std::string name){ // gets a node from a node name+
      return ref_node(name_reference(name));
    }
    int tree_size(){return global_refenrence_counter.load();}
    template<typename... Args>
    JieDian_t<YanSe_t> *yield_node_step(Args&&... args){
      set_current_node(ref_node(d_state_ref)->step(args...));
      return ref_node(d_state_ref);
    }
    template<typename... Args>
    CanKao_t yield_next_reference(Args&&... args){
      return yield_node_step(args...)->self_reference;
    }
    template<typename... Args>
    YanSe_t yield_next_color(Args&&... args){
      return yield_node_step(args...)->color;
    }
    template<typename... Args>
    std::vector<CanKao_t> yield_reference_sequence(std::vector<bool> arguments_sequence){
      std::vector<CanKao_t> reference_sequence;
      for(bool arg : arguments_sequence) {
        reference_sequence.push_back(yield_next_reference(arg));}
      return reference_sequence;
    }
    std::vector<YanSe_t> yield_color_sequence(std::vector<bool> arguments_sequence){
      std::vector<YanSe_t> color_sequence;
      for(bool arg : arguments_sequence) {
        color_sequence.push_back(yield_next_color(arg));}
      return color_sequence;
    }
  };
}
// --- --- --- --- --- --- --- --- --- --- --- --- ---
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 
// --- --- --- --- --- --- --- --- EXAMPLE OF GOWI --- --- --- --- --- --- --- --- --- --- --- --- 
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 
// --- --- --- --- --- --- --- --- --- --- --- --- --- 
void gowi_action(KAOSHI::YanSe_t *payload_color, KAOSHI::YanSe_t *current_node_color){
  payload_color->honor += current_node_color->honor;
  payload_color->phrase = std::string(payload_color->phrase.c_str()) + std::string(current_node_color->phrase.c_str());
}
void void_action(KAOSHI::YanSe_t *payload_color, KAOSHI::YanSe_t *current_node_color){
}
void end_action(KAOSHI::YanSe_t *payload_color, KAOSHI::YanSe_t *current_node_color) {
  gowi_action(payload_color, current_node_color);
  fprintf(stdout,"Gowi's Life results:\n");
  fprintf(stdout,"   Honor:  %f\n",payload_color->honor);
  fprintf(stdout,"   Phrase: %s\n",payload_color->phrase.c_str());
  exit(0);
}
// --- --- --- --- --- --- --- --- --- --- --- --- --- 
bool boolean_step_transition(KAOSHI::YanSe_t *payload_color, bool transition) {
  return transition;
}
bool end_step_transition(KAOSHI::YanSe_t *payload_color, bool transition) {
  return true;
}
// --- --- --- --- --- --- --- --- --- --- --- --- --- 
int main(){
// --- --- --- --- --- --- --- --- --- --- --- --- --- 
  // --- defned as:
  std::list<KAOSHI::Jiyi_t<KAOSHI::YanSe_t>> definition;
  // ---  --- --- --- ---      --- --- --- --- ---             --- --- --- ---     --- --- --- --- --- --- --- --- --- 
  //                      |                  TRANSITIONS                       |   COLOR 
  //                      |    --- --- --- --- --- --- --- --- --- ---         |    
  //                      |    NAME         POSITIVE          NEGATIVE         |   Gowi's life
  //                       --- --- ---      --- --- ---       --- --- ---      --- --- --- --- --- --- ---
  definition.push_back({   "0x9a",          "END_NODE",       "END_NODE",       { 0xFF,"\t revolution success \n"              }});
  definition.push_back({   "0x9b",          "END_NODE",       "END_NODE",       {-0x01,"\t revolution failure \n"              }});
  definition.push_back({   "REVOLUTION",    "0x9a",           "0x9b",           { 0x00,"\t [revolution event encounter:] \n"   }});
  definition.push_back({   "0x8a",          "REVOLUTION",     "REVOLUTION",     { 0x01,"\t success in oldhood \n"              }});
  definition.push_back({   "0x8b",          "REVOLUTION",     "REVOLUTION",     {-0x01,"\t failure at oldhood \n"              }});
  definition.push_back({   "FINALHOOD",     "0x8a",           "0x8b",           { 0x00,"\t [finalhood event encounter:] \n"    }});
  definition.push_back({   "0x7a",          "FINALHOOD",      "FINALHOOD",      { 0x01,"\t success in parenthood \n"           }});
  definition.push_back({   "0x7b",          "FINALHOOD",      "FINALHOOD",      {-0x01,"\t failure at parenthood \n"           }});
  definition.push_back({   "PARENTHOOD",    "0x7a",           "0x7b",           { 0x00,"\t [parenthood event encounter:] \n"   }});
  definition.push_back({   "0x6a",          "PARENTHOOD",     "PARENTHOOD",     { 0x01,"\t success in childhood \n"            }});
  definition.push_back({   "0x6b",          "PARENTHOOD",     "PARENTHOOD",     {-0x01,"\t failure at childhood \n"            }});
  definition.push_back({   "CHILDHOOD",     "0x6a",           "0x6b",           { 0x00,"\t [childhood event encounter:] \n"    }});
  definition.push_back({   "0x5a",          "CHILDHOOD",      "CHILDHOOD",      { 0x01,"\t success in love making \n"          }});
  definition.push_back({   "0x5b",          "CHILDHOOD",      "CHILDHOOD",      {-0x01,"\t failure at love making \n"          }});
  definition.push_back({   "LOVEHOOD",      "0x5a",           "0x5b",           { 0x00,"\t [love making event encounter:] \n"  }});
  definition.push_back({   "0x4a",          "LOVEHOOD",       "LOVEHOOD",       { 0x01,"\t success in drinking \n"             }});
  definition.push_back({   "0x4b",          "LOVEHOOD",       "LOVEHOOD",       {-0x01,"\t failure at drinking \n"             }});
  definition.push_back({   "DRINKHOOD",     "0x4a",           "0x4b",           { 0x00,"\t [drinking event encounter:] \n"     }});
  definition.push_back({   "0x3a",          "DRINKHOOD",      "DRINKHOOD",      { 0x01,"\t success in dancing \n"              }});
  definition.push_back({   "0x3b",          "DRINKHOOD",      "DRINKHOOD",      {-0x01,"\t failure at dancing \n"              }});
  definition.push_back({   "DANCEHOOD",     "0x3a",           "0x3b",           { 0x00,"\t [dancing event encounter:] \n"      }});
  definition.push_back({   "0x2a",          "DANCEHOOD",      "DANCEHOOD",      { 0x01,"\t success in painting \n"             }});
  definition.push_back({   "0x2b",          "DANCEHOOD",      "DANCEHOOD",      {-0x01,"\t failure at painting \n"             }});
  definition.push_back({   "PAINTHOOD",     "0x2a",           "0x2b",           { 0x00,"\t [painting event encounter:] \n"     }});
  definition.push_back({   "0x1a",          "PAINTHOOD",      "PAINTHOOD",      { 0x01,"\t success in dialog \n"               }});
  definition.push_back({   "0x1b",          "PAINTHOOD",      "PAINTHOOD",      {-0x01,"\t failure at dialog \n"               }});
  definition.push_back({   "DIALOG",        "0x1a",           "0x1b",           { 0x00,"\t [dialog event encounter:] \n"       }});
  definition.push_back({   "START_NODE",    "DIALOG",         "DIALOG",         { 0x0F,"\t subject Gowi lives \n"              }});
  // --- --- --- --- --- --- ---     --- --- --- --- ---             --- --- ---    --- --- --- --- --- --- --- --- --- --- --- --- 
  // --- --- --- --- --- --- --- --- --- --- --- --- --- 
  KAOSHI::CanKao_t* end_reference = new KAOSHI::CanKao_t({
    KAOSHI::global_refenrence_counter.load(), // unique_reference
    "END_NODE", // name
    NULL,        // node_positive
    NULL          // node_negative
  });
  // --- --- --- --- --- --- --- --- --- --- --- --- --- 
  KAOSHI::Shu d_shu = KAOSHI::Shu();                              // create Tree
  // --- --- --- --- --- --- --- --- --- --- --- --- --- 
  d_shu.new_node(                                                 // create End node
    end_reference->name, 
    {0x00,"subject Gowi dies a long life."},
    end_action,
    end_step_transition,
    end_reference,
    end_reference
  );
  // --- --- --- --- --- --- --- --- --- --- --- --- --- 
  for(KAOSHI::Jiyi_t<KAOSHI::YanSe_t> def : definition){          // allocate all nodes
    d_shu.new_node(
      def.name, 
      def.color,
      gowi_action,
      boolean_step_transition,
      d_shu.name_reference(def.positive_name),
      d_shu.name_reference(def.negative_name)
    );
  }
  // --- --- --- --- --- --- --- --- --- --- --- --- --- 
  std::cout<<"The total number of nodes is: "<<d_shu.tree_size()<<std::endl;
  // --- --- --- --- --- --- --- --- --- --- --- --- --- 
  d_shu.set_current_node(d_shu.name_reference("START_NODE"));     // set initial state
  // --- --- --- --- --- --- --- --- --- --- --- --- --- 
  std::vector<bool> arguments_sequence=std::vector<bool>({
    true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true
  });
  std::vector<KAOSHI::YanSe_t> results_sequence = d_shu.yield_color_sequence(arguments_sequence);
  // --- --- --- --- --- --- --- --- --- --- --- --- --- 
  return 0;
}