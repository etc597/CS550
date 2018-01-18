#ifndef Standard_Delegate_hpp
#define Standard_Delegate_hpp

template <typename Return, typename Arg = Return>
struct Delegate {};

template <typename Return, typename ...Arguments>
class Delegate<Return(*)(Arguments...)>
{
public:
  using FunctionSignature = Return(*)(Arguments...);
  using Invoker = Return(*)(void*, Arguments...);

  // None of this for you.
  inline Delegate(const Delegate &aDelegate) = delete;

  template <typename ObjectType, typename FunctionType, FunctionType aFunction>
  static Return Caller(void *aObject, Arguments... aArguments)
  {
    return (static_cast<ObjectType*>(aObject)->*aFunction)(aArguments...);
  }

  template <typename ObjectType>
  inline Delegate(ObjectType *aObject, Invoker aInvoker)
    : mObject(static_cast<void*>(aObject))
    , mCallerFunction(aInvoker)
  {
  }

  inline Delegate(Delegate &&aDelegate)
    : mObject(aDelegate.mObject)
    , mCallerFunction(aDelegate.mCallerFunction)
  {

  }

  inline Return Invoke(Arguments... aArguments)
  {
    return mCallerFunction(mObject, aArguments...);
  }

  template<typename tFunctionType, tFunctionType aFunction, typename tObjectType>
  static Delegate From(tObjectType * aObj)
  {
    return Delegate(aObj, Caller<tObjectType, tFunctionType, aFunction>);
  }

  void *GetCallingObject() const {
    return mObject;
  }

  Invoker GetCallerFunction() const {
    return mCallerFunction;
  }

protected:

  void *mObject;
  Invoker mCallerFunction;
};


#endif
