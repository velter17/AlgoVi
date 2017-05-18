/**
 * Project   AlgoVi
 *
 * @file     ClassGenerators.hpp
 * @author   Dmytro Sadovyi
 * @date     12.05.2017
 */

#pragma once

#define ADD_ATTRIBUTE(classname, name, type) \
   public: \
      const type& get##name() const \
      { return m##name; } \
      classname& set##name(const type& item) \
      { \
         m##name = item; \
         return *this; \
      } \
   private: \
      type m##name;

#define SINGLETON_CLASS_BASE(classname) \
    public: \
        static classname& getInstance() \
        { \
            static classname instance; \
            return instance; \
        } \
    private: \
        classname(const classname& other) = delete; \
        classname operator=(const classname& other) = delete;

#define SINGLETON_CLASS_BASE_CONSTR(classname) \
    SINGLETON_CLASS_BASE(classname) \
    classname() {}
