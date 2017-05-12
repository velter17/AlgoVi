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
