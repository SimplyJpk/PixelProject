#pragma once
#include "InputKeyCodes.h"
#include "ISerializable.h"

struct InputConfig : public ISerializable
{
protected:
   void Save(cereal::JSONOutputArchive out_archive) override;
   void Load(cereal::JSONInputArchive in_archive) override;

   SaveTypes SaveType() override
   {
      return SaveTypes::Json;
   }
public:
   std::string FilePath() override
   {
      return std::string("InputConfig");
   }

   MouseCode m_fire1 = MouseCode::MouseLeft;
   MouseCode m_fire2 = MouseCode::MouseRight;

   KeyCode m_move_left[2]{ KeyCode::A, KeyCode::Left };
   KeyCode m_move_north[2]{ KeyCode::W, KeyCode::Up};
   KeyCode m_move_east[2]{ KeyCode::D, KeyCode::Right };
   KeyCode m_move_south[2]{ KeyCode::S, KeyCode::Down };

   KeyCode m_action1 = KeyCode::Space;
};

inline void InputConfig::Save(cereal::JSONOutputArchive out_archive)
{
   out_archive(::cereal::make_nvp("Fire1", m_fire1));
   out_archive(::cereal::make_nvp("Fire2", m_fire2));
   out_archive(::cereal::make_nvp("Left", m_move_left));
   out_archive(::cereal::make_nvp("Up", m_move_north));
   out_archive(::cereal::make_nvp("Right", m_move_east));
   out_archive(::cereal::make_nvp("Down", m_move_south));
   out_archive(::cereal::make_nvp("Action", m_action1));
}

inline void InputConfig::Load(cereal::JSONInputArchive in_archive)
{
   in_archive(::cereal::make_nvp("Fire1", m_fire1));
   in_archive(::cereal::make_nvp("Fire2", m_fire2));
   in_archive(::cereal::make_nvp("Left", m_move_left));
   in_archive(::cereal::make_nvp("Up", m_move_north));
   in_archive(::cereal::make_nvp("Right", m_move_east));
   in_archive(::cereal::make_nvp("Down", m_move_south));
   in_archive(::cereal::make_nvp("Action", m_action1));
}
