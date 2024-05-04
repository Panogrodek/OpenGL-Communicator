#pragma once

namespace pl {
	enum class KeyCode {
		NONE = 0,
		PRESS = 1,
		REPEAT = 2,
		RELEASE = 3,
	};
}

namespace priv {
	class Renderer;
}

namespace priv {
	class InputGuard {
	public:
		void Init();

		void Lock(int c);

		pl::KeyCode GetKeyCode(int c);
		pl::KeyCode& operator[](int index);
	private:
		void UpdateKey(int key, int action);
		friend class priv::Renderer;
		std::unordered_map<int, pl::KeyCode> m_keys;
	};
}

namespace pl {
	inline priv::InputGuard inputGuard;
}