#include "PatchTestLib.h"
#include <sstream>
#include <algorithm>

using namespace NSPatchTestLib;

static std::vector<std::string> split(const std::string& s, char delim)
{
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim))
    {
        result.push_back(item);
    }

    return result;
}

void PatchTestLib::Init(IFont* font,
                         ISoundEffect* SE,
                         ISprite* sprCursor,
                         ISprite* sprBackground)
{
    m_font = font;
    m_SE = SE;
    m_sprCursor = sprCursor;
    m_sprBackground = sprBackground;
}

void NSPatchTestLib::PatchTestLib::SetInventoryList(const std::vector<StoreItem>& arg)
{
    m_leftList = arg;
}

void NSPatchTestLib::PatchTestLib::SetStorehouseList(const std::vector<StoreItem>& arg)
{
    m_rightList = arg;
}

void NSPatchTestLib::PatchTestLib::MoveFromInventoryToStorehouse(const int id, const int subid)
{
    auto it = std::find_if(m_leftList.begin(), m_leftList.end(),
                           [&](const StoreItem& x)
                           {
                               return x.GetId() == id && x.GetSubId() == subid;
                           });

    if (it == m_leftList.end())
    {
        throw std::exception();
    }

    StoreItem item = *it;
    m_leftList.erase(it);
    m_rightList.push_back(item);

    std::sort(m_rightList.begin(), m_rightList.end(),
              [&](const StoreItem& left, const StoreItem& right)
              {
                  if (left.GetId() < right.GetId())
                  {
                      return true;
                  }
                  else if (left.GetId() > right.GetId())
                  {
                      return false;
                  }
                  // left.GetId() == right.GetId()
                  else
                  {
                      if (left.GetSubId() < right.GetSubId())
                      {
                          return true;
                      }
                      //  left.GetSubId() >= right.GetSubId()
                      else
                      {
                          return false;
                      }
                  }
              });
}

void NSPatchTestLib::PatchTestLib::MoveFromStorehouseToInventory(const int id, const int subid)
{
    auto it = std::find_if(m_rightList.begin(), m_rightList.end(),
                           [&](const StoreItem& x)
                           {
                               return x.GetId() == id && x.GetSubId() == subid;
                           });

    if (it == m_rightList.end())
    {
        throw std::exception();
    }

    StoreItem item = *it;
    m_rightList.erase(it);
    m_leftList.push_back(item);

    std::sort(m_leftList.begin(), m_leftList.end(),
              [&](const StoreItem& left, const StoreItem& right)
              {
                  if (left.GetId() < right.GetId())
                  {
                      return true;
                  }
                  else if (left.GetId() > right.GetId())
                  {
                      return false;
                  }
                  // left.GetId() == right.GetId()
                  else
                  {
                      if (left.GetSubId() < right.GetSubId())
                      {
                          return true;
                      }
                      //  left.GetSubId() >= right.GetSubId()
                      else
                      {
                          return false;
                      }
                  }
              });
}

std::string PatchTestLib::Up()
{
    if (m_eFocus == eFocus::LEFT)
    {
        if (m_leftSelect >= 1)
        {
            m_leftSelect--;
            m_SE->PlayMove();
        }
        // �J�[�\������ԏ�ɂ���Ƃ��ɏ�{�^���������ꂽ��
        // �J�[�\���͂��̂܂܂Ń��X�g�����Ɉړ�����
        if (m_leftCursor != 0)
        {
            m_leftCursor--;
        }
        else if (m_leftCursor == 0)
        {
            if (m_leftSelect != 0)
            {
                m_leftBegin--;
            }
        }
    }
    else if (m_eFocus == eFocus::RIGHT)
    {
        if (m_rightSelect >= 1)
        {
            m_rightSelect--;
            m_SE->PlayMove();
        }
        // �J�[�\������ԏ�ɂ���Ƃ��ɏ�{�^���������ꂽ��
        // �J�[�\���͂��̂܂܂Ń��X�g�����Ɉړ�����
        if (m_rightCursor != 0)
        {
            m_rightCursor--;
        }
        else if (m_rightCursor == 0)
        {
            if (m_rightSelect != 0)
            {
                m_rightBegin--;
            }
        }
    }
    return "";
}

std::string PatchTestLib::Down()
{
    if (m_eFocus == eFocus::LEFT)
    {
        if (m_leftSelect <= (int)m_leftList.size() - 2)
        {
            m_leftSelect++;
            m_SE->PlayMove();
        }

        // �J�[�\������ԉ��ɂ���Ƃ��ɉ��{�^���������ꂽ��
        // �J�[�\���͂��̂܂܂Ń��X�g����Ɉړ�����
        if (m_leftCursor != PANEL_ROW_MAX - 1)
        {
            m_leftCursor++;
        }
        else if (m_leftCursor == PANEL_ROW_MAX - 1)
        {
            if (m_leftSelect != (int)m_leftList.size() - 1)
            {
                m_leftBegin++;
            }
        }
    }
    else if (m_eFocus == eFocus::RIGHT)
    {
        if (m_rightSelect <= (int)m_rightList.size() - 2)
        {
            m_rightSelect++;
            m_SE->PlayMove();
        }

        // �J�[�\������ԉ��ɂ���Ƃ��ɉ��{�^���������ꂽ��
        // �J�[�\���͂��̂܂܂Ń��X�g����Ɉړ�����
        if (m_rightCursor != PANEL_ROW_MAX - 1)
        {
            m_rightCursor++;
        }
        else if (m_rightCursor == PANEL_ROW_MAX - 1)
        {
            if (m_rightSelect != (int)m_rightList.size() - 1)
            {
                m_rightBegin++;
            }
        }
    }
    return "";
}

std::string PatchTestLib::Right()
{
    if (m_eFocus == eFocus::LEFT)
    {
        m_eFocus = eFocus::RIGHT;
        m_SE->PlayMove();
    }
    return std::string();
}

std::string PatchTestLib::Left()
{
    if (m_eFocus == eFocus::RIGHT)
    {
        m_eFocus = eFocus::LEFT;
        m_SE->PlayMove();
    }
    return std::string();
}

std::string PatchTestLib::Into()
{
    std::string result;
    if (m_eFocus == eFocus::LEFT)
    {
        result = "left:";
        result += m_leftList.at(m_leftSelect).GetName();
        result += ":" + std::to_string(m_leftList.at(m_leftSelect).GetId());
        result += ":" + std::to_string(m_leftList.at(m_leftSelect).GetSubId());
        m_SE->PlayClick();
    }
    else if (m_eFocus == eFocus::RIGHT)
    {
        result = "right:";
        result += m_rightList.at(m_rightSelect).GetName();
        result += ":" + std::to_string(m_rightList.at(m_rightSelect).GetId());
        result += ":" + std::to_string(m_rightList.at(m_rightSelect).GetSubId());
        m_SE->PlayClick();
    }
    return result;
}

std::string PatchTestLib::Back()
{
    std::string result;
    result = "EXIT";
    m_SE->PlayBack();

    return result;
}

std::string NSPatchTestLib::PatchTestLib::Next()
{
    if (m_eFocus == eFocus::LEFT)
    {
        if (m_leftSelect <= (int)m_leftList.size() - 2)
        {
            m_leftSelect++;
            m_SE->PlayMove();
        }

        // �J�[�\������ԉ��ɂ���Ƃ��ɉ��{�^���������ꂽ��
        // �J�[�\���͂��̂܂܂Ń��X�g����Ɉړ�����
        if (m_leftCursor != PANEL_ROW_MAX - 1)
        {
            m_leftCursor++;
        }
        else if (m_leftCursor == PANEL_ROW_MAX - 1)
        {
            if (m_leftSelect != (int)m_leftList.size() - 1)
            {
                m_leftBegin++;
            }
        }
    }
    else if (m_eFocus == eFocus::RIGHT)
    {
        if (m_rightSelect <= (int)m_rightList.size() - 2)
        {
            m_rightSelect++;
            m_SE->PlayMove();
        }

        // �J�[�\������ԉ��ɂ���Ƃ��ɉ��{�^���������ꂽ��
        // �J�[�\���͂��̂܂܂Ń��X�g����Ɉړ�����
        if (m_rightCursor != PANEL_ROW_MAX - 1)
        {
            m_rightCursor++;
        }
        else if (m_rightCursor == PANEL_ROW_MAX - 1)
        {
            if (m_rightSelect != (int)m_rightList.size() - 1)
            {
                m_rightBegin++;
            }
        }
    }
    return "";
}

std::string NSPatchTestLib::PatchTestLib::Previous()
{
    if (m_eFocus == eFocus::LEFT)
    {
        if (m_leftSelect >= 1)
        {
            m_leftSelect--;
            m_SE->PlayMove();
        }
        // �J�[�\������ԏ�ɂ���Ƃ��ɏ�{�^���������ꂽ��
        // �J�[�\���͂��̂܂܂Ń��X�g�����Ɉړ�����
        if (m_leftCursor != 0)
        {
            m_leftCursor--;
        }
        else if (m_leftCursor == 0)
        {
            if (m_leftSelect != 0)
            {
                m_leftBegin--;
            }
        }
    }
    else if (m_eFocus == eFocus::RIGHT)
    {
        if (m_rightSelect >= 1)
        {
            m_rightSelect--;
            m_SE->PlayMove();
        }
        // �J�[�\������ԏ�ɂ���Ƃ��ɏ�{�^���������ꂽ��
        // �J�[�\���͂��̂܂܂Ń��X�g�����Ɉړ�����
        if (m_rightCursor != 0)
        {
            m_rightCursor--;
        }
        else if (m_rightCursor == 0)
        {
            if (m_rightSelect != 0)
            {
                m_rightBegin--;
            }
        }
    }
    return "";
}

void PatchTestLib::CursorOn(const int x, const int y)
{
    int previousCursor = m_leftCursor;
    if (LEFT_PANEL_STARTX < x && x <= LEFT_PANEL_STARTX + PANEL_WIDTH)
    {
        if (LEFT_PANEL_STARTY < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 1)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 0;
            m_leftSelect = 0 + m_leftBegin;
        }
        else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 1 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 2)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 1;
            m_leftSelect = 1 + m_leftBegin;
        }
        else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 2 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 3)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 2;
            m_leftSelect = 2 + m_leftBegin;
        }
        else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 3 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 4)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 3;
            m_leftSelect = 3 + m_leftBegin;
        }
        else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 4 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 5)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 4;
            m_leftSelect = 4 + m_leftBegin;
        }
        else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 5 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 6)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 5;
            m_leftSelect = 5 + m_leftBegin;
        }
        else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 6 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 7)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 6;
            m_leftSelect = 6 + m_leftBegin;
        }
        else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 7 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 8)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 7;
            m_leftSelect = 7 + m_leftBegin;
        }
        else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 8 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 9)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 8;
            m_leftSelect = 8 + m_leftBegin;
        }
        else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 9 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 10)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 9;
            m_leftSelect = 9 + m_leftBegin;
        }
    }
    if (previousCursor != m_leftCursor)
    {
        m_SE->PlayMove();
    }

    int previousRightCursor = m_rightCursor;
    if (RIGHT_PANEL_STARTX < x && x <= RIGHT_PANEL_STARTX + PANEL_WIDTH)
    {
        if (RIGHT_PANEL_STARTY < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 1)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 0;
            m_rightSelect = 0 + m_rightBegin;
        }
        else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 1 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 2)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 1;
            m_rightSelect = 1 + m_rightBegin;
        }
        else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 2 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 3)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 2;
            m_rightSelect = 2 + m_rightBegin;
        }
        else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 3 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 4)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 3;
            m_rightSelect = 3 + m_rightBegin;
        }
        else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 4 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 5)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 4;
            m_rightSelect = 4 + m_rightBegin;
        }
        else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 5 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 6)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 5;
            m_rightSelect = 5 + m_rightBegin;
        }
        else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 6 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 7)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 6;
            m_rightSelect = 6 + m_rightBegin;
        }
        else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 7 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 8)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 7;
            m_rightSelect = 7 + m_rightBegin;
        }
        else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 8 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 9)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 8;
            m_rightSelect = 8 + m_rightBegin;
        }
        else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 9 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 10)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 9;
            m_rightSelect = 9 + m_rightBegin;
        }
    }
    if (previousRightCursor != m_rightCursor)
    {
        m_SE->PlayMove();
    }
}

std::string PatchTestLib::Click(const int x, const int y)
{
    std::string result;
    m_SE->PlayClick();
    if (m_eFocus == eFocus::LEFT)
    {
        if (LEFT_PANEL_STARTX < x && x <= LEFT_PANEL_STARTX + PANEL_WIDTH)
        {
            if (LEFT_PANEL_STARTY < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 1)
            {
                m_leftCursor = 0;
                m_leftSelect = 0 + m_leftBegin;
                result = Into();
            }
            else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 1 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 2)
            {
                m_leftCursor = 1;
                m_leftSelect = 1 + m_leftBegin;
                result = Into();
            }
            else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 2 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 3)
            {
                m_leftCursor = 2;
                m_leftSelect = 2 + m_leftBegin;
                result = Into();
            }
            else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 3 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 4)
            {
                m_leftCursor = 3;
                m_leftSelect = 3 + m_leftBegin;
                result = Into();
            }
            else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 4 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 5)
            {
                m_leftCursor = 4;
                m_leftSelect = 4 + m_leftBegin;
                result = Into();
            }
            else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 5 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 6)
            {
                m_leftCursor = 5;
                m_leftSelect = 5 + m_leftBegin;
                result = Into();
            }
            else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 6 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 7)
            {
                m_leftCursor = 6;
                m_leftSelect = 6 + m_leftBegin;
                result = Into();
            }
            else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 7 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 8)
            {
                m_leftCursor = 7;
                m_leftSelect = 7 + m_leftBegin;
                result = Into();
            }
            else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 8 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 9)
            {
                m_leftCursor = 8;
                m_leftSelect = 8 + m_leftBegin;
                result = Into();
            }
            else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 9 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 10)
            {
                m_leftCursor = 9;
                m_leftSelect = 9 + m_leftBegin;
                result = Into();
            }
        }
    }
    else if (m_eFocus == eFocus::RIGHT)
    {
        if (RIGHT_PANEL_STARTX < x && x <= RIGHT_PANEL_STARTX + PANEL_WIDTH)
        {
            if (RIGHT_PANEL_STARTY < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 1)
            {
                m_rightCursor = 0;
                m_rightSelect = 0 + m_rightBegin;
                result = Into();
            }
            else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 1 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 2)
            {
                m_rightCursor = 1;
                m_rightSelect = 1 + m_rightBegin;
                result = Into();
            }
            else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 2 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 3)
            {
                m_rightCursor = 2;
                m_rightSelect = 2 + m_rightBegin;
                result = Into();
            }
            else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 3 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 4)
            {
                m_rightCursor = 3;
                m_rightSelect = 3 + m_rightBegin;
                result = Into();
            }
            else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 4 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 5)
            {
                m_rightCursor = 4;
                m_rightSelect = 4 + m_rightBegin;
                result = Into();
            }
            else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 5 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 6)
            {
                m_rightCursor = 5;
                m_rightSelect = 5 + m_rightBegin;
                result = Into();
            }
            else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 6 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 7)
            {
                m_rightCursor = 6;
                m_rightSelect = 6 + m_rightBegin;
                result = Into();
            }
            else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 7 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 8)
            {
                m_rightCursor = 7;
                m_rightSelect = 7 + m_rightBegin;
                result = Into();
            }
            else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 8 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 9)
            {
                m_rightCursor = 8;
                m_rightSelect = 8 + m_rightBegin;
                result = Into();
            }
            else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 9 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 10)
            {
                m_rightCursor = 9;
                m_rightSelect = 9 + m_rightBegin;
                result = Into();
            }
        }
    }
    return result;
}

void PatchTestLib::Draw()
{
    // �w�i
    m_sprBackground->DrawImage(0, 0);

    // �㕔���̍��Ɂu�C���x���g���v�A�E���Ɂu�q�Ɂv�ƕ\������
    m_font->DrawText_("�H�ރ��X�g", 205, 110);

    m_font->DrawText_("�H�ޖ�", 155, 160, 64);

    m_font->DrawText_("�p�b�`�e�X�g�̃��X�g", 855, 110);

    m_font->DrawText_("�H�ޖ�", 455, 160, 64);
    m_font->DrawText_("�˗���", 655, 160, 64);
    m_font->DrawText_("�J�n��", 855, 160, 64);
    m_font->DrawText_("������", 1055, 160, 64);
    m_font->DrawText_("���茋��", 1255, 160, 64);

    // ���̗�̃C���x���g����\��
    if ((int)m_leftList.size() >= PANEL_ROW_MAX)
    {
        for (int i = m_leftBegin; i < m_leftBegin + PANEL_ROW_MAX; ++i)
        {
            m_font->DrawText_(m_leftList.at(i).GetName(),
                              LEFT_PANEL_STARTX + PANEL_PADDINGX,
                              LEFT_PANEL_STARTY + PANEL_PADDINGY + ((i - m_leftBegin) * PANEL_HEIGHT));
        }
    }
    else
    {
        for (std::size_t i = 0; i < m_leftList.size(); ++i)
        {
            m_font->DrawText_(m_leftList.at(i).GetName(),
                              LEFT_PANEL_STARTX + PANEL_PADDINGX,
                              LEFT_PANEL_STARTY + PANEL_PADDINGY + ((int)i * PANEL_HEIGHT));
        }
    }

    // �E�̗�̑q�ɂ�\��
    if ((int)m_rightList.size() >= PANEL_ROW_MAX)
    {
        for (int i = m_rightBegin; i < m_rightBegin + PANEL_ROW_MAX; ++i)
        {
            m_font->DrawText_(m_rightList.at(i).GetName(),
                              RIGHT_PANEL_STARTX + PANEL_PADDINGX,
                              RIGHT_PANEL_STARTY + PANEL_PADDINGY + ((i - m_rightBegin) * PANEL_HEIGHT));
        }
    }
    else
    {
        for (std::size_t i = 0; i < m_rightList.size(); ++i)
        {
            m_font->DrawText_(m_rightList.at(i).GetName(),
                              RIGHT_PANEL_STARTX + PANEL_PADDINGX,
                              RIGHT_PANEL_STARTY + PANEL_PADDINGY + ((int)i * PANEL_HEIGHT));
        }
    }

    // �J�[�\���̕\��
    if (m_eFocus == eFocus::LEFT)
    {
        m_sprCursor->DrawImage(80, 218 + (m_leftCursor * 60));
    }
    else if (m_eFocus == eFocus::RIGHT)
    {
        m_sprCursor->DrawImage(680, 218 + (m_rightCursor * 60));
    }


}

void NSPatchTestLib::StoreItem::SetId(const int arg)
{
    m_id = arg;
}

int NSPatchTestLib::StoreItem::GetId() const
{
    return m_id;
}

void NSPatchTestLib::StoreItem::SetSubId(const int arg)
{
    m_idSub = arg;
}

int NSPatchTestLib::StoreItem::GetSubId() const
{
    return m_idSub;
}

void NSPatchTestLib::StoreItem::SetName(const std::string& arg)
{
    m_name = arg;
}

std::string NSPatchTestLib::StoreItem::GetName() const
{
    return m_name;
}
