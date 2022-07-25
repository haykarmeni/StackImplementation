#include <gtest/gtest.h>
#include "stack.h"

TEST(StackTest, TestingDefaultCtor) {
    my::Stack<int> myStack;
    EXPECT_EQ(myStack.size(), 0);
}

TEST(StackTest, TestingDefaultCtorWithCapacity) {
    my::Stack<int> myStack(10);
    EXPECT_EQ(myStack.size(), 0);
}

TEST(StackTest, TestingCopyCtor) {
    my::Stack<int> myStack{1,2,3};
    auto myStackCopy(myStack);
    EXPECT_EQ(myStackCopy.top(), 3);
    myStackCopy.pop();
    EXPECT_EQ(myStackCopy.top(), 2);
    myStackCopy.pop();
    EXPECT_EQ(myStackCopy.top(), 1);
    EXPECT_EQ(myStackCopy.size(), 1);
}

TEST(StackTest, TestingMoveCtor) {
    my::Stack<int> myStack{1,2,3};
    my::Stack<int> myStackCopy(std::move(myStack));
    EXPECT_EQ(myStackCopy.top(), 3);
    myStackCopy.pop();
    EXPECT_EQ(myStackCopy.top(), 2);
    myStackCopy.pop();
    EXPECT_EQ(myStackCopy.top(), 1);
    EXPECT_EQ(myStackCopy.size(), 1);
}

TEST(StackTest, TestingCopyAssignOp) {
    my::Stack<int> myStack{1,2,3};
    my::Stack<int> lhs;
    lhs = myStack;
    EXPECT_EQ(lhs.top(), 3);
    lhs.pop();
    EXPECT_EQ(lhs.top(), 2);
    lhs.pop();
    EXPECT_EQ(lhs.top(), 1);
    EXPECT_EQ(lhs.size(), 1);
}

TEST(StackTest, TestingMoveAssignOp) {
    my::Stack<int> myStack{1,2,3};
    my::Stack<int> lhs;
    lhs = std::move(myStack);
    EXPECT_EQ(lhs.top(), 3);
    lhs.pop();
    EXPECT_EQ(lhs.top(), 2);
    lhs.pop();
    EXPECT_EQ(lhs.top(), 1);
    EXPECT_EQ(lhs.size(), 1);
}

TEST(StackTest, TestingPush) {
    my::Stack<int> myStack;
    int x = 1;
    EXPECT_EQ(myStack.size(), myStack.capacity());//both should be 0
    myStack.push(x);
    EXPECT_EQ(myStack.size(), myStack.capacity());//both should be 1
    myStack.push(x + x);
    EXPECT_EQ(myStack.size(), 2);
    EXPECT_EQ(myStack.capacity(), 3);
}

TEST(StackTest, TestingTop) {
    my::Stack<int> myStack;
    my::Stack<int> myStack1{1,2,3};
    const my::Stack<int> myStack2{1,2,3};
    int& top = myStack1.top();
    top = 10;
    EXPECT_EQ(myStack1.top(), 10);
    try {
        top = myStack.top();
        FAIL() << "This is empty stack";
    }
    catch(const std::logic_error& err) {
        EXPECT_EQ(err.what(), std::string{"Empty stack!"});
    }
    catch(...) {
        FAIL() << "Expected std::logic_error";
    }
    const int tmp = myStack2.top();
    EXPECT_EQ(tmp, 3);
}

TEST(StackTest, TestingPop) {
    my::Stack<int> myStack{1,2,3};
    while(myStack.size()) {
        myStack.pop();
    }
    try {
        myStack.pop();
        FAIL() << "This is empty stack";
    }
    catch(const std::underflow_error& err) {
        EXPECT_EQ(err.what(), std::string{"Pop from empty stack!"});
    }
    catch(...) {
        FAIL() << "Expected std::underflow_error";
    }
}

