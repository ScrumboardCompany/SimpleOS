#pragma once

#ifndef _MATH_
#define _MATH_

namespace SimpleOS {

	template<class _Ty>
	_Ty min(const _Ty& a, const _Ty& b) {
		return (a < b) ? a : b;
	}

	template<class _Ty>
	_Ty max(const _Ty& a, const _Ty& b) {
		return (a > b) ? a : b;
	}
}

#endif // _MATH_