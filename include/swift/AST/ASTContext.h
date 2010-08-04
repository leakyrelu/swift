//===--- ASTContext.h - AST Context Object ----------------------*- C++ -*-===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2015 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See http://swift.org/LICENSE.txt for license information
// See http://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//
//
// This file defines the ASTContext interface.
//
//===----------------------------------------------------------------------===//

#ifndef SWIFT_AST_ASTCONTEXT_H
#define SWIFT_AST_ASTCONTEXT_H

namespace llvm {
  class BumpPtrAllocator;
  class SourceMgr;
  class StringRef;
  template <typename T> class FoldingSet;
}

namespace swift {
  class Type;
  class AliasType;
  class TupleType;
  class FunctionType;
  class NamedDecl;
  class Identifier;
  class TupleTypeElt;

/// ASTContext - This object creates and owns the AST objects.
class ASTContext {
  ASTContext(const ASTContext&);           // DO NOT IMPLEMENT
  void operator=(const ASTContext&);       // DO NOT IMPLEMENT
  llvm::BumpPtrAllocator *Allocator;
  
  void *IdentifierTable; // llvm::StringMap<char>
  void *AliasTypes;      // llvm::StringMap<AliasType*>
  void *TupleTypes;      // llvm::FoldingSet<TupleType>
  void *FunctionTypes;   // DenseMap<std::pair<Type*,Type*>, FunctionType*>
public:
  ASTContext(llvm::SourceMgr &SourceMgr);
  ~ASTContext();
  
  /// SourceMgr - The source manager object.
  llvm::SourceMgr &SourceMgr;

  /// Allocate - Allocate memory from the ASTContext bump pointer.
  void *Allocate(unsigned long Bytes, unsigned Alignment);

  /// getIdentifier - Return the uniqued and AST-Context-owned version of the
  /// specified string.
  Identifier getIdentifier(llvm::StringRef Str);
    
  //===--------------------------------------------------------------------===//
  // Type manipulation routines.
  //===--------------------------------------------------------------------===//
  
  /// getCanonicalType - Get the canonicalized version of a type, stripping off
  /// sugar like argument names and type aliases.
  Type *getCanonicalType(Type *T);
  
  // Builtin type and simple types that are used frequently.
  Type * const VoidType;      /// VoidType - This is 'void', aka "()"
  Type * const DependentTy;   /// DependentTy - Type is dependent on context.
  Type * const IntType;       /// IntType - This is 'int'.

  /// getNamedType - This method does a lookup for the specified type name.  If
  /// no type with the specified name exists, null is returned.
  Type *getNamedType(Identifier Name);
  
  /// getAliasType - Return a new alias type.  This returns null if there is a
  /// conflict with an already existing alias type of the same name.
  AliasType *getAliasType(Identifier Name, Type *Underlying);
                          
  /// getTupleType - Return the uniqued tuple type with the specified elements.
  TupleType *getTupleType(const TupleTypeElt *Fields,
                          unsigned NumFields);
  
  /// getFunctionType - Return a uniqued function type with the specified
  /// input and result.
  FunctionType *getFunctionType(Type *Input, Type *Result);
};
  
} // end namespace swift

#endif
