#ifndef CONTEXT_H
#define CONTEXT_H

class Context {
public:
    Context() = default;
    ~Context() = default;

    static void init(void);
    static void destroy(void);
};


#endif