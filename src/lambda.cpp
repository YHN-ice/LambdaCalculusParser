
#include "lambda.hpp"
#include "store.hpp"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#ifdef TRACK_TIME
#include <chrono>
using clk = std::chrono::system_clock;
using sec = std::chrono::duration<double>;
#endif
#ifdef USAGE
std::unordered_map<std::string, int> counter;
void print_call_counter(){
  printf("call counter:\n");
  for (auto &p : counter)
  {
    printf("<%s>:(%d)\n", p.first.c_str(), p.second);
  }
}
#endif
// L::L(const L &obj){
//   type = obj.type;
//   key = obj.key;
//   var = obj.var;
//   para = obj.para;
//   body = obj.body;
//   callee = obj.callee;
//   arg = obj.arg;
// }
L::L()
{
  key = var = para = body = callee = arg = "";
  type = VAR;
}
L::L(std::string var) : var(var)
{
  key = para = body = callee = arg = "";
  type = VAR;
}
L::L(std::string head, std::string tail, bool is_func)
{
  key = var = para = body = callee = arg = "";
  if (is_func)
  {
    para = head;
    body = tail;
    type = FUN;
  }
  else
  {
    callee = head;
    arg = tail;
    type = APP;
  }
}

void L::dump() const
{
  switch (type)
  {
  case FUN:
    printf("($%s.", para.c_str());
    load_symbol(body).dump();
    printf(")");
    break;
  case APP:
    printf("(");
    load_symbol(callee).dump();
    printf(" ");
    load_symbol(arg).dump();
    printf(")");
    break;

  default:
    printf("%s", var.c_str());
    break;
  }
}
std::string L::to_string() const
{
  std::string buf;
  switch (type)
  {
  case FUN:
    buf = "($" + para + "." + load_symbol(body).to_string() + ")";
    break;
  case APP:
    buf = "(" + load_symbol(callee).to_string() + " " + load_symbol(arg).to_string() + ")";
    break;
  default:
    buf = var;
    break;
  }
  return buf;
}
std::string L::emit_immediate() const
{
  std::string T = "var";
  switch (type)
  {
  case VAR:
    T = "var";
    break;
  case FUN:
    T = "fun";
    break;

  default:
    T = "app";
    break;
  }
  return T + "\t$" + para + ".\t" + body + ";\t" + callee + "<-\t" + arg + "\t" + "<"+key+">";
}


/* a generator must has two params and body starting with a if-stmt, then branch is base return value */
bool L::is_generator(){
  if(type != FUN) return false;
  L func = load_symbol(body);
  if (func.type!=FUN) return false;
  L body = load_symbol(func.body); /* (G I a) (I) ((M a (fact( I P a))) */
  if (body.type!= APP) return false;
  L base_cond = load_symbol(body.callee);
  if(base_cond.type!=APP) return false;
  L base_res = load_symbol(base_cond.arg);
  if(l2i(base_res)==-1) return false;
  L binary_op = load_symbol(base_cond.callee);
  if (binary_op.type!=APP) return false;
  L lhs = load_symbol(binary_op.callee);
  if (lhs.type!=APP||lhs.callee!="G") return false;
  L recursive_part = load_symbol(load_symbol(body.arg).arg);
  if(recursive_part.callee!=para) return false;
  return true;
}
L L::apply()
{
  L res;
  #ifdef TRACK_TIME
  const auto before = clk::now();
  #endif
  /* check if it is a recursive pure call: Y G Num */
  L callee_l = load_symbol(callee);
  /* here we are comparing reference name, which is dependent on the demo.in file */
  if (callee_l.type == APP &&
      callee_l.callee == "Y" &&
      l2i(load_symbol(arg)) != -1 &&
      load_symbol(callee_l.arg).is_generator())
  {
    res = recursive_apply();
  }
  else
  {
    res = naive_apply();
  }
  #ifdef TRACK_TIME
  const sec duration = clk::now() - before;
  printf("\n%f apply TIME ELAPSED\n", duration.count());
  #endif
  return res;
}

L L::naive_apply(){
  Impl impl = to_impl();
  impl.apply();
  return from_impl(impl);
}

L L::recursive_apply(){
#ifndef PROD
  printf("Yambda trick...\n");
  printf("branched to recursive apply");
  dump();
#endif
  L generator = load_symbol(load_symbol(callee).arg);
  L arg_l = load_symbol(arg);
  Recursive r(generator.to_impl());
  return from_impl(Impl::i2l(r.reduce(l2i(arg_l))));
}
Impl L::to_impl(){
  if(type==VAR) {
    return Impl(VAR,var,{});
  } else if(type==FUN){
    return Impl(FUN, para, {load_symbol(body).to_impl()});
  } else {
    return Impl(APP, "", {load_symbol(callee).to_impl(),load_symbol(arg).to_impl()});
  }
}
L L::from_impl(Impl l){
  if(l.type==VAR){
    return L(l.var);
  } else if(l.type==FUN) {
    L body = from_impl(l.data[0]);
    return L(l.var, KEY(body));
  } else {
    L cle = from_impl(l.data[0]);
    L arg = from_impl(l.data[1]);
    return L(KEY(cle), KEY(arg), false);
  }
}



L L::i2l(int i){
  return from_impl(Impl::i2l(i));
}
int L::l2i(L l){
  return l.to_impl().l2i();
  // if(l.key == "O") {
  //   return 0;
  // } else if(l.key == "I") {
  //   return 1;
  // } else if(l.type == APP && l.callee == "S"){
  //   return 1 + l2i(load_symbol(l.arg));
  // } else {
  //   return -1;
  // }
}

bool L::is_ephemeral(){
  if(key.empty())
    return true;
  if (key.size() != EPHEMERAL_NAME_LEN)
  {
    return false;
  }
  for(auto &c:key){
    if(!((c>='0' && c<='9')||( c>='a' && c<='z'))) return false;
  }
  return true;
}
void L::cleanup(){
  if (type == VAR || !is_ephemeral())
    return;
#ifndef PROD
  printf("***cleaning up %s\n", emit_immediate().c_str());
#endif
  if(type == FUN){
    load_symbol(body).cleanup();
    if(key.size()) {
      remove_symbol(key);
    }
  } else {
    load_symbol(callee).cleanup();
    load_symbol(arg).cleanup();
    if(key.size()) {
      remove_symbol(key);
    }
  }
}

Impl::Impl(int t, std::string v, std::vector<Impl> d):type(t), var(v), data(d){}
void Impl::dump() const{
  switch (type)
  {
  case FUN:
    printf("($%s.", var.c_str());
    data[0].dump();
    printf(")");
    break;
  case APP:
    printf("(");
    data[0].dump();
    printf(" ");
    data[1].dump();
    printf(")");
    break;

  default:
    printf("%s", var.c_str());
    break;
  }
}

void Impl::apply(){
#ifdef USAGE
  counter["apply"] += 1;
  // printf("call apply\n");
#endif
  while(reduce())
    ;
}
bool Impl::reduce()
{
  bool res = false;
  if (type == VAR)
    return false;
  if(type==FUN){
    res = data[0].reduce();
  } else {
    if (data[0].type == FUN) {
      data[0].data[0].replace(data[0].var, data[1]);
      Impl applied = data[0].data[0];
      *this = applied;
      res = true;
    }
    else
    {
      bool res_h = data[0].reduce();
      bool res_t = data[1].reduce();
      res = res_h || res_t;
    }
  }
  return res;
}

bool Impl::replace(std::string placeholder, Impl arg){
  bool res = false;
  if (type == FUN)
  {
    if(placeholder == var)
      return res;
    res = data[0].replace(placeholder, arg);
  }
  else if (type == APP)
  {
    bool res_h = data[0].replace(placeholder, arg);
    bool res_t =  data[1].replace(placeholder, arg);
    res = res_h || res_t;
  }
  else
  {
    if(var==placeholder){
      *this = arg;
      res = true;
    }
  }
  return res;
}
Impl Impl::i2l(int i){
  // S = $w.$n.$m.n((w n)m)
  static Impl s = Impl(FUN, "w", {Impl(FUN, "n", {Impl(FUN, "m", {Impl(APP, "", {Impl(VAR, "n", {}), Impl(APP, "", {Impl(APP, "", {Impl(VAR, "w", {}), Impl(VAR, "n", {})}), Impl(VAR, "m", {})})})})})});
  static std::unordered_map<int, Impl> cache;

  if(cache.find(i)!=cache.end())
    return cache.find(i)->second;

  if (i == 0)
  {
    cache.emplace(0, Impl(FUN, "j", {Impl(FUN, "i", {Impl(VAR, "i", {})})}));
  }
  else cache.emplace(i, Impl(APP, "", {s, i2l(i - 1)})) ;
  return cache.find(i)->second;
}
bool Impl::is_true(){
  apply();
  if(type!=FUN)
    return false;
  if(data[0].type!=FUN)
    return false;
  if(data[0].data[0].type!=VAR)
    return false;
  if(data[0].data[0].var!=var)
    return false;
  return true;
}
bool Impl::is_false(){
  apply();
  if(type!=FUN)
    return false;
  if(data[0].type!=FUN)
    return false;
  if(data[0].data[0].type!=VAR)
    return false;
  if(data[0].data[0].var!=data[0].var)
    return false;
  return true;
}

int Impl::l2i(Impl l){
  if(l.is_false())
    return 0;
  if(l.is_true())
    return 1;
  if (l.type != FUN)
      return -1;
  if (l.data[0].type != FUN)
    return -1;
  if (l.data[0].data[0].type != APP)
    return -1;
  if (l.data[0].data[0].data[0].type != VAR)
    return -1;
  if (l.data[0].data[0].data[0].var != l.var)
    return -1;
  l.data[0].data[0] = l.data[0].data[0].data[1];
  return 1+l2i(l);
}
int Impl::l2i(){
  Impl clone(*this);
  clone.apply();
  return l2i(clone);
}
#ifdef MUL_OPTM
bool Impl::isMUL() const{
  // M = $x.$y.$z.x(y z)
  std::string x, y, z;
  if (type != FUN)
    return false;
  else
    x = var;
  if (data[0].type != FUN)
    return false;
  else
    y = data[0].var;
  if (data[0].data[0].type != FUN)
    return false;
  else
    z = data[0].data[0].var;
  if (data[0].data[0].data[0].type != APP)
    return false;

  if (data[0].data[0].data[0].data[0].type != VAR)
    return false;

  if (data[0].data[0].data[0].data[0].var != x)
    return false;

  if (data[0].data[0].data[0].data[1].type != APP)
    return false;

  if (data[0].data[0].data[0].data[1].data[0].type != VAR)
    return false;
  if (data[0].data[0].data[0].data[1].data[0].var != y)
    return false;
  if (data[0].data[0].data[0].data[1].data[1].type != VAR)
    return false;
  if (data[0].data[0].data[0].data[1].data[1].var != z)
    return false;
  return true;
}
#endif
// M 4 r
// M 4 (M 3 r)
// $z.4(M 3 r)z
// $z.4(M 3 r)z
// M ? r
// $z.?(r)z

Recursive::Recursive(Impl gen):generator(gen){}
void Recursive::collapse(){
#ifdef MUL_OPTM
  /* optimization specific to MUL */
  Impl &in_stack_post_app = outer.back().data[0].data[0].data[0].data[0]; // M a
  if (in_stack_post_app.data[0].isMUL())
  {
    printf("isMUL\n");
    int a = std::stoi(outer.back().data[0].data[0].var);
    int b = std::stoi(outer.front().data[0].data[0].var);
    int res = a * b;
    // printf("%d %d collapsed res is %d\n", a, b, res);
    outer.front().data[0].data[0].data[1] = Impl::i2l(res);
    outer.front().data[0].data[0].var = std::to_string(res);
    outer.pop_back();
    return;
  }
#endif
#ifdef USAGE
  counter["collapse"] += 1;
  // printf("call collapse\n");
#endif
  Impl dense(FUN, "r", {Impl(APP, "", {outer[0], Impl(APP, "", {outer[1], Impl(VAR, "r", {})})})});
  dense.apply();
#ifndef PROD
  printf("\n=============DENSED=============\n");
  outer[0].dump();
  printf("\n-------------DENSED-------------\n");
  outer[1].dump();
  printf("\n>>>>>>>>>>>>>DENSED>>>>>>>>>>>>>\n");
  dense.dump();
  printf("\n+++++++++++++DENSED+++++++++++++\n");
#endif
  outer.clear();
  outer.push_back(dense);
}
int Recursive::reduce(int i){
#ifdef USAGE
  counter["reduce"] += 1;
  // printf("call reduce\n");
#endif
  const Impl &func = generator.data[0];
  const Impl &body = generator.data[0].data[0];
  while(true){

#ifndef PROD
    printf("iterative reduce in Recur i=%d\n", i);
    if (i == 0)
    {
      printf("body dump\n");
      body.dump();
    }
#endif
    Impl condition = Impl(APP, "", {Impl(FUN, func.var, {body.data[0].data[0]}), Impl::i2l(i)});

#ifdef TRACK_TIME
    const auto before_cond = clk::now();
#endif
    condition.apply();
#ifdef TRACK_TIME
    const sec cond_duration = clk::now() - before_cond;
    printf("\n%f condition TIME ELAPSED\n", cond_duration.count());
#endif
#ifndef PROD
    printf("condition... %d/%d:", condition.is_true(),condition.is_false());
    condition.dump();
#endif
    if (condition.is_true())
    {
      Impl base = body.data[0].data[1];
      return outer_apply(base.l2i());
    }
    else if (condition.is_false())
    {
      Impl arg_next = Impl(APP, "", {Impl(FUN, func.var, {body.data[1].data[1].data[1]}),Impl::i2l(i)});
#ifdef TRACK_TIME
      const auto before_arg_next = clk::now();
#endif
      arg_next.apply();
#ifdef TRACK_TIME
      const sec arg_next_duration = clk::now() - before_arg_next;
      printf("\n\t%f arg next TIME ELAPSED\n", arg_next_duration.count());
#endif

#ifndef PROD
      printf("arg_next...:");
      arg_next.dump();
      printf("\n");
#endif
      Impl post_app = body.data[1].data[0];            // M a
      Impl post_fun = Impl(FUN, func.var, {post_app}); // $a. M a
#ifdef MUL_OPTM
      std::string ivar = "";
      if (body.data[1].data[0].data[0].isMUL())
        ivar = std::to_string(i);
      Impl outer_layer = Impl(APP, ivar, {post_fun, Impl::i2l(i)}); // M 3
#else
      Impl outer_layer = Impl(APP, "", {post_fun, Impl::i2l(i)}); // M 3
#endif
      Impl outer_layer_func = Impl(FUN, "r", {Impl(APP, "", {outer_layer, Impl(VAR,"r",{})})}); // $r. (M 3)<-r
      outer.push_back(outer_layer_func);
      if(outer.size()==2){
#ifdef TRACK_TIME
        const auto before_collapse = clk::now();
#endif
        collapse();
#ifdef TRACK_TIME
        const sec collapse_duration = clk::now() - before_collapse;
        printf("\n\t\t%f collapse TIME ELAPSED\n", collapse_duration.count());
#endif
      }
      i = arg_next.l2i();
    }
    else
    {
      printf("!!!!!!!ERROR ARGUMENT, condition result to NON-T/F\n");
      return -1;
    }
  }
}

int Recursive::outer_apply(int i) {
#ifdef USAGE
  counter["outer_apply"] += 1;
  // printf("call outer_apply\n");
#endif
  if(outer.empty())
    return i;
  else {
    assert(outer.size() == 1);
    Impl res = Impl(APP, "", {outer[0], Impl::i2l(i)});
#ifndef PROD
    printf("^^^^^^^^^^^^^^^^^^\n");
    outer[0].dump();
#endif
    res.apply();
    return res.l2i();
  }
}

int L::call_lambda(std::string func, int arg)
{
  L f("Y", func, false);
  L argument = i2l(arg);
  L ap(KEY(f), KEY(argument), false);
#ifndef PROD
  printf("=========lambda to reduce, with arg%d=========", arg);
  ap.dump();
#endif
  L res_l = ap.apply();
#ifndef PROD
  res_l.dump();
#endif
  int res = l2i(res_l);
  res_l.cleanup();
  return res;
}
