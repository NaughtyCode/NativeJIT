/*
xJITExample1 is showing redundant adds when collapsing field pointers.
x        === Pass3 ===
x        add r9, 1234
x        add r9, [r8]
x        // release r8
x        mul rdx, 40
x        add rcx, rdx
x        // release rdx
x    ==> add rcx, 16
x        mov rcx, [rcx + 8]
x    ==> add rcx, 16
x        add r9, [rcx + 8]
x        // release rcx
x        mov rax, r9
x        ret
x        // release r9


x Implement BinaryNode operation.

RegisterFile: naming for reserved/available is reversed from bit values.
Ensure that base pointer is reserved correctly.
  Assert if a parameter maps to base.
  Prevent base from being added to free list.
  Initialize base correctly in prologue.
Ensure the RSP is reserved correctly.
  Above items.
  Also, IsBasePointer() should include StackPointer as well. Rename to IsReserved?

Add XMM registers to available list.

When CastNode is added, IsTrue should be made into a CastNode.

Cannot spill registers that are used to hold cached CSE values.

Cached conditionals need some way to recreate the correct flags or to use a different flag.
Conditional introduce a new requirement on the evaluation of common subexpressions.
  It is possible that a common subexpression need never be evaluated. Consider
    if (version == 0)
      a;
    else if (version == 1)
      a + b;
    else
      b + c;
  In one case, a will not need to be evaluated and in another case, c will not need
  to be evaluated.
  Recommend lazily evaluating CSEs, instead of evaluating them all first.

Is there some opportunity for code reuse between IsTrue<T> and RelationalOperator<T, JCC>
  Simply approach would be to define IsTrue<T> as RelationalOperatorNode<T, JNZ> with Immediate(0).
  Perhaps factoring out base classes would allow the more efficient or-operation in IsTrue<> while
  retaining the cmp-operation in RelationalOperatorNode.

Is there some opportunity for code reuse between BinaryNode::CodeGenValue and RelationalOperatorNode::CodeGenFlags()?
High duplication between RelationalOperatorNode<T, JCC>::CodeGenValue() and IsTrue<T>::CodeGenValue().


Carefully check register allocation and labelling in ConditionalNode.h.

FlagExpressionNode
    (a relop b)?THEN:ELSE
    (bool)?THEN:ELSE
    (a && b && !c)?THEN:ELSE

    Equals
    NotEquals
    GT
    GTE
    LT
    LTE
    &&  ==> (a)?b:FALSE
    || ==> (!a)?b:TRUE
    !

    Option I
      All nodes are flag expression nodes and implement CodeGenFlags()
    Option II
      One must use a cast node to convert value nodes to flags nodes
      Flag nodes can still have a CodeGenValue method
    Option III
      Somehow find a way to specialize generic nodes so that the bool typed variants are also Flag nodes.

Is there some way to reduce code duplication between BinaryNode::CodeGenValue() and RelationalOperatorNode::CodeGenFlags()?
Reduce code duplication around IsCached() case in all the implementations of CodeGenValue().

operator==() for Register.
Register needs to be templated by signed/unsigned (ISSIGNED)
RXX sizes 1, 2, 4
  Need to deal with the fact that only a subset of all registers are valid.
  Need to deal with register size casting problem in IndirectNode<T>::CodeGenValue where base register is reused for result.
  May want to try to collapse the Register templates if possible. Two issues:
    Printing register names. This can be handled with an array of array of char*.
    Excluding certain registers (e.g. RSI cannot by 1-byte). Need to verify this limitation.
  Cast operators.
  May want to reserve RAX for assisting in code generation (e.g loading a byte into RSI).
  Registers need to be sign aware.
XMM registers
  Need to deal with register size casting problem in IndirectNode<T>::CodeGenValue where base register is reused for result.
RegisterFile should exclude certain machine-specific registerrs (e.g. RSP).

Is return node required to sign-extend values with byte sizes 1, 2, and 4?
Does indirect load of 1-byte impact other bytes in register?

Support for more than 4 parameters. (or at least assert)

x ExpressionTree::GetAvailableRegisterCount needs to handle all types of registers.
x Implement and use ExpressionTree::ReleaseRegister()
x Use templates to simplify implementation of ExpressionTree::AllocateRegister().
x Implement ExpressionNodeFactory with arena allocator.
x Implement factory method for add integer to pointer.
Implement call node with various parameters.
Implement support for doubles.
Implement Prologue and Epilogue with patching for locals.
Implement register dumping and local variables.
Implement unwind information.
Unit test that executes generated code.
Pack and unpack operations.
Comprehensive unit tests.
Examples.
Implement real code generator.

Conditional nodes
  Boolean expressions need special flag mode optimization so that boolean value doesn't have to be generated.

Call node
  ParameterHome
  ParameterHomeSequence
    Determine amount of parameter home space required and allocate on stack
    Evaluate each parameter home expression and transfer to correct register
    Sometimes need to save and restore a register that was bumped for a parameter home
    Make the call
    Restore bumped registers
    Deallocate space

Sequence node - like C++ comma operator.

x Rename Immediate.h ==> ImmediateNode.h
x Rename FieldPointer.h ==> FieldPointerNode.h
x Rename Parameter.h ==> ParameterNode.h
x Rename Return.h ==> ReturnNode.h
x Rename other node classes
Are Deref and IndirectNode always the same? (or does Factory::FieldPointer() sometimes need to create Indirect?

*/
