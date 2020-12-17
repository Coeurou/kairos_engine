#pragma once

#include <core/error.h>
#include <math/rect.h>
#include <graphics/sprite.h>

namespace kairos {

class painter {
    friend void clear(const painter& painter, const color& color);
    friend void draw(painter& painter, const sprite& sprite);
    friend void draw(painter& painter, const array<sprite>& sprites);
    friend void draw(painter& painter, const rectf& rect);
    friend void draw(painter& painter, const array<rectf>& rects);
    friend void draw(painter& painter, const linef& line);
    friend void draw(painter& painter, const array<linef>& lines);

    template <class T>
    using disable_copy = std::enable_if_t<!std::is_same_v<std::decay_t<T>, painter>>*;

  public:
    /** Member functions */

    /** Construct a painter.
    * Will do a dynamic allocation for its internal implementation.
    */
    template <class T> explicit painter(T&& renderer, disable_copy<T> = nullptr) {
        my_self = new painter_impl<T>(std::forward<T>(renderer));
    }

    /** Copy constructor. Use copy on write (ref_count is incremented). */
    painter(const painter& p);

    /** Copy assignement operator. Use copy on write (ref_count is incremented). */
    painter& operator=(const painter& p);

    /** Move constructor. Move p::painter_t pointer. */
    painter(painter&& p) noexcept;

    /** Move assignement operator. Move w::painter_t pointer. */
    painter& operator=(painter&& p) noexcept;

    /** Destructor. Delete my_self where there is no other reference of painter_t pointer. */
    ~painter();

  private:
      /** Type erasure details */

      enum class drawable {
        line,
        rectangle,
        sprite
      };

      /** painter_t represents the painter interface. */
      struct painter_t {
          /** Call destructor of derived class. */
          virtual void destruct() = 0;
          virtual void clear(const color& color) = 0;
          virtual void draw(drawable shape_type, const void* shape, size_t count) = 0;
          virtual ~painter_t() {}

          int ref_count = 0;
      };

      /** painter_impl implements the painter interface, by forwarding calls to type-erased concrete
      * painter. */
      template <class T> struct painter_impl final : public painter_t {

          /** Constructor. Forward arguments to T::Constructor. */
          template <class... Args>
          explicit painter_impl(Args&&... args) : renderer(std::forward<Args>(args)...) {
              static_assert(!std::is_same_v<std::decay_t<T>, painter_impl>,
                  "Should use proper copy or move constructor");
              ref_count++;
          }

          /** Call destructor. */
          void destruct() override { this->~painter_impl(); }

          void clear(const color& color) override {
              kairos::clear(renderer, color);
          }

          void draw(drawable shape_type, const void* shape, size_t count) override {
            switch (shape_type) {
            case drawable::line: {
                if (count > 1) {
                    kairos::draw(renderer, *reinterpret_cast<const array<linef>*>(shape));
                }
                else {
                    kairos::draw(renderer, *reinterpret_cast<const linef*>(shape));
                }
                break;
            }
            case drawable::rectangle: {
                if (count > 1) {
                    kairos::draw(renderer, *reinterpret_cast<const array<rectf>*>(shape));
                }
                else {
                    kairos::draw(renderer, *reinterpret_cast<const rectf*>(shape));
                }
                break;
            }
            case drawable::sprite: {
                if (count > 1) {
                    kairos::draw(renderer, *reinterpret_cast<const array<sprite>*>(shape));
                }
                else {
                    kairos::draw(renderer, *reinterpret_cast<const sprite*>(shape));
                }
                break;
            }
            default:
                ASSERT_MSG(false, "[painter_impl::draw] unknown shape to draw");
                break;
            }
          }

          /** Type instance where the logic of a painter is implemented */
          T renderer;
      };

      painter_t* my_self = nullptr;
};

/** Non-member functions */

void clear(const painter& painter, const color& color);

void draw(painter& painter, const sprite& sprite);
void draw(painter& painter, const array<sprite>& sprites);

void draw(painter& painter, const rectf& rect);
void draw(painter& painter, const array<rectf>& rects);

void draw(painter& painter, const linef& line);
void draw(painter& painter, const array<linef>& lines);

} // namespace kairos