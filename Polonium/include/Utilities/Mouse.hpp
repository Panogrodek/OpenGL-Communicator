#pragma once

namespace priv {
	class Renderer;
}

namespace priv {
	class Mouse {
	public:
		glm::vec2 GetOffset() const;
		void BlockOffset();
	private:
		friend class priv::Renderer;
		glm::vec2 m_offset;
	};
}

namespace pl {
	inline priv::Mouse mouse;
}