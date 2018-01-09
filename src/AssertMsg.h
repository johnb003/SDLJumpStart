#ifndef ASSERT_MSG_H
#define ASSERT_MSG_H

#ifndef NDEBUG
#define ASSERT_MSG(Expr, Msg) CustomAssertMsg(#Expr, Expr, __FILE__, __LINE__, Msg)
#else
#define ASSERT_MSG(Expr, Msg) ;
#endif

void CustomAssertMsg(const char* expr_str, bool expr, const char* file, int line, const char* msg);

#endif  // ASSERT_MSG_H
