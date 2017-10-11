#ifndef TIGERAPP_H
#define TIGERAPP_H

#include "MooseApp.h"

class TigerApp;

template <>
InputParameters validParams<TigerApp>();

class TigerApp : public MooseApp
{
public:
  TigerApp(InputParameters parameters);
  virtual ~TigerApp();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void registerObjectDepends(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
  static void associateSyntaxDepends(Syntax & syntax, ActionFactory & action_factory);
};

#endif /* TIGERAPP_H */
