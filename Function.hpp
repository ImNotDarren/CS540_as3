//
//  Function.h
//  FunctionWrapper
//
//  Created by Darren on 12/9/21.
//
#include <iostream>
#ifndef Function_h
#define Function_h

namespace cs540 {

class BadFunctionCall {
public:
    BadFunctionCall() {};
};

template <typename> class Function;

template <typename FunctionType, typename ResultType, typename... ArgumentTypes> class HelpFunction;

template <typename FunctionType, typename ResultType, typename... ArgumentTypes> class HelpFunction<FunctionType, ResultType (ArgumentTypes...)> : public Function<ResultType (ArgumentTypes...)> {
public:
    FunctionType fun;
    
    HelpFunction (FunctionType fun) : fun(fun) {};
    
    ResultType operator()(ArgumentTypes... args) override {
        return fun(args...);
    };
    
    explicit operator bool() const {
        if(this) {
            return true;
        }else{
            return false;
        }
    };
    
};

template <typename ResultType, typename ... ArgumentTypes> class Function<ResultType(ArgumentTypes...)> {
public:
    Function<ResultType(ArgumentTypes...)> *fun;
    
    Function() : fun(nullptr) {};
    
    template <typename FunctionType> Function(FunctionType f) : fun(new HelpFunction<FunctionType, ResultType (ArgumentTypes...)>(f)) {};
    
    Function(const Function &f) : fun(f.fun) {};
    
    Function &operator=(const Function & f) {
        fun = f.fun;
        return *this;
    };
    
    virtual ResultType operator()(ArgumentTypes... args) {
        if(fun) {
            return (*fun)(args...);
        }else{
            throw BadFunctionCall();
        }
    };
    
    explicit operator bool() const {
        if(fun) {
            return true;
        }else{
            return false;
        }
    };
    
    ~Function() {};
};

template <typename ResultType, typename ... ArgumentTypes>
bool operator==(const Function<ResultType(ArgumentTypes...)> &f, std::nullptr_t) {
    return !f;
};

template <typename ResultType, typename ... ArgumentTypes>
bool operator==(std::nullptr_t, const Function<ResultType(ArgumentTypes...)> & f) {
    return !f;
};

template <typename ResultType, typename ... ArgumentTypes>
bool operator!=(const Function<ResultType(ArgumentTypes...)> &f, std::nullptr_t) {
    return bool(f);
};

template <typename ResultType, typename ... ArgumentTypes>
bool operator!=(std::nullptr_t, const Function<ResultType(ArgumentTypes...)> & f) {
    return bool(f);
};

}



#endif /* Function_h */
