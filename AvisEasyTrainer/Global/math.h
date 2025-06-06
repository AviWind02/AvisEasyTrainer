#pragma once

namespace g_feature{
	 namespace g_math {


		inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) {
			return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y);
		}

		inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) {
			return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
		}

		inline ImVec2 operator*(const ImVec2& lhs, float scalar) {
			return ImVec2(lhs.x * scalar, lhs.y * scalar);
		}

		inline ImVec2 operator*(float scalar, const ImVec2& rhs) {
			return ImVec2(rhs.x * scalar, rhs.y * scalar);
		}

		inline ImVec2 operator/(const ImVec2& lhs, float scalar) {
			return ImVec2(lhs.x / scalar, lhs.y / scalar);
		}

		inline ImVec2& operator+=(ImVec2& lhs, const ImVec2& rhs) {
			lhs.x += rhs.x;
			lhs.y += rhs.y;
			return lhs;
		}

		inline ImVec2& operator-=(ImVec2& lhs, const ImVec2& rhs) {
			lhs.x -= rhs.x;
			lhs.y -= rhs.y;
			return lhs;
		}

		inline ImVec2& operator*=(ImVec2& lhs, float scalar) {
			lhs.x *= scalar;
			lhs.y *= scalar;
			return lhs;
		}

		inline ImVec2& operator/=(ImVec2& lhs, float scalar) {
			lhs.x /= scalar;
			lhs.y /= scalar;
			return lhs;
		}

	}
}