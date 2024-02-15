#pragma once

#ifndef RAPORTGENCORE_EXPORT
#ifdef RaportGenCore_EXPORTS
/* We are building this library */
#define RAPORTGENCORE_EXPORT __declspec(dllexport)
#else
#ifdef RaportGenCore_TESTS
#define RAPORTGENCORE_EXPORT
#else
/* We are using this library */
#define RAPORTGENCORE_EXPORT __declspec(dllimport)
#endif
#endif
#endif
