#ifndef TIGERTESTAPP_H
#define TIGERTESTAPP_H

#include "MooseApp.h"

class TigerTestApp;

template <>
InputParameters validParams<TigerTestApp>();

class TigerTestApp : public MooseApp
{
public:
  TigerTestApp(InputParameters parameters);
  virtual ~TigerTestApp();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
};

#endif /* TIGERTESTAPP_H */
