// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: rpcmessage.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_rpcmessage_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_rpcmessage_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3007000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3007001 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_rpcmessage_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_rpcmessage_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_rpcmessage_2eproto;
namespace netcore {
class RpcMessage;
class RpcMessageDefaultTypeInternal;
extern RpcMessageDefaultTypeInternal _RpcMessage_default_instance_;
}  // namespace netcore
PROTOBUF_NAMESPACE_OPEN
template<> ::netcore::RpcMessage* Arena::CreateMaybeMessage<::netcore::RpcMessage>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace netcore {

enum ErrorCode : int {
  NO_ERROR = 0,
  WRONG_PROTO = 1,
  NO_SERVICE = 2,
  NO_METHOD = 3
};
bool ErrorCode_IsValid(int value);
constexpr ErrorCode ErrorCode_MIN = NO_ERROR;
constexpr ErrorCode ErrorCode_MAX = NO_METHOD;
constexpr int ErrorCode_ARRAYSIZE = ErrorCode_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* ErrorCode_descriptor();
template<typename T>
inline const std::string& ErrorCode_Name(T enum_t_value) {
  static_assert(::std::is_same<T, ErrorCode>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function ErrorCode_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    ErrorCode_descriptor(), enum_t_value);
}
inline bool ErrorCode_Parse(
    const std::string& name, ErrorCode* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<ErrorCode>(
    ErrorCode_descriptor(), name, value);
}
// ===================================================================

class RpcMessage :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:netcore.RpcMessage) */ {
 public:
  RpcMessage();
  virtual ~RpcMessage();

  RpcMessage(const RpcMessage& from);
  RpcMessage(RpcMessage&& from) noexcept
    : RpcMessage() {
    *this = ::std::move(from);
  }

  inline RpcMessage& operator=(const RpcMessage& from) {
    CopyFrom(from);
    return *this;
  }
  inline RpcMessage& operator=(RpcMessage&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const RpcMessage& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const RpcMessage* internal_default_instance() {
    return reinterpret_cast<const RpcMessage*>(
               &_RpcMessage_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(RpcMessage* other);
  friend void swap(RpcMessage& a, RpcMessage& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline RpcMessage* New() const final {
    return CreateMaybeMessage<RpcMessage>(nullptr);
  }

  RpcMessage* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<RpcMessage>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const RpcMessage& from);
  void MergeFrom(const RpcMessage& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  #if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  #else
  bool MergePartialFromCodedStream(
      ::PROTOBUF_NAMESPACE_ID::io::CodedInputStream* input) final;
  #endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  void SerializeWithCachedSizes(
      ::PROTOBUF_NAMESPACE_ID::io::CodedOutputStream* output) const final;
  ::PROTOBUF_NAMESPACE_ID::uint8* InternalSerializeWithCachedSizesToArray(
      ::PROTOBUF_NAMESPACE_ID::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(RpcMessage* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "netcore.RpcMessage";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_rpcmessage_2eproto);
    return ::descriptor_table_rpcmessage_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required string service = 1;
  bool has_service() const;
  void clear_service();
  static const int kServiceFieldNumber = 1;
  const std::string& service() const;
  void set_service(const std::string& value);
  void set_service(std::string&& value);
  void set_service(const char* value);
  void set_service(const char* value, size_t size);
  std::string* mutable_service();
  std::string* release_service();
  void set_allocated_service(std::string* service);

  // required string method = 2;
  bool has_method() const;
  void clear_method();
  static const int kMethodFieldNumber = 2;
  const std::string& method() const;
  void set_method(const std::string& value);
  void set_method(std::string&& value);
  void set_method(const char* value);
  void set_method(const char* value, size_t size);
  std::string* mutable_method();
  std::string* release_method();
  void set_allocated_method(std::string* method);

  // optional bytes message = 3;
  bool has_message() const;
  void clear_message();
  static const int kMessageFieldNumber = 3;
  const std::string& message() const;
  void set_message(const std::string& value);
  void set_message(std::string&& value);
  void set_message(const char* value);
  void set_message(const void* value, size_t size);
  std::string* mutable_message();
  std::string* release_message();
  void set_allocated_message(std::string* message);

  // @@protoc_insertion_point(class_scope:netcore.RpcMessage)
 private:
  class HasBitSetters;

  // helper for ByteSizeLong()
  size_t RequiredFieldsByteSizeFallback() const;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr service_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr method_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr message_;
  friend struct ::TableStruct_rpcmessage_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// RpcMessage

// required string service = 1;
inline bool RpcMessage::has_service() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void RpcMessage::clear_service() {
  service_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  _has_bits_[0] &= ~0x00000001u;
}
inline const std::string& RpcMessage::service() const {
  // @@protoc_insertion_point(field_get:netcore.RpcMessage.service)
  return service_.GetNoArena();
}
inline void RpcMessage::set_service(const std::string& value) {
  _has_bits_[0] |= 0x00000001u;
  service_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:netcore.RpcMessage.service)
}
inline void RpcMessage::set_service(std::string&& value) {
  _has_bits_[0] |= 0x00000001u;
  service_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:netcore.RpcMessage.service)
}
inline void RpcMessage::set_service(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  _has_bits_[0] |= 0x00000001u;
  service_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:netcore.RpcMessage.service)
}
inline void RpcMessage::set_service(const char* value, size_t size) {
  _has_bits_[0] |= 0x00000001u;
  service_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:netcore.RpcMessage.service)
}
inline std::string* RpcMessage::mutable_service() {
  _has_bits_[0] |= 0x00000001u;
  // @@protoc_insertion_point(field_mutable:netcore.RpcMessage.service)
  return service_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* RpcMessage::release_service() {
  // @@protoc_insertion_point(field_release:netcore.RpcMessage.service)
  if (!has_service()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000001u;
  return service_.ReleaseNonDefaultNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void RpcMessage::set_allocated_service(std::string* service) {
  if (service != nullptr) {
    _has_bits_[0] |= 0x00000001u;
  } else {
    _has_bits_[0] &= ~0x00000001u;
  }
  service_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), service);
  // @@protoc_insertion_point(field_set_allocated:netcore.RpcMessage.service)
}

// required string method = 2;
inline bool RpcMessage::has_method() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void RpcMessage::clear_method() {
  method_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  _has_bits_[0] &= ~0x00000002u;
}
inline const std::string& RpcMessage::method() const {
  // @@protoc_insertion_point(field_get:netcore.RpcMessage.method)
  return method_.GetNoArena();
}
inline void RpcMessage::set_method(const std::string& value) {
  _has_bits_[0] |= 0x00000002u;
  method_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:netcore.RpcMessage.method)
}
inline void RpcMessage::set_method(std::string&& value) {
  _has_bits_[0] |= 0x00000002u;
  method_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:netcore.RpcMessage.method)
}
inline void RpcMessage::set_method(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  _has_bits_[0] |= 0x00000002u;
  method_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:netcore.RpcMessage.method)
}
inline void RpcMessage::set_method(const char* value, size_t size) {
  _has_bits_[0] |= 0x00000002u;
  method_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:netcore.RpcMessage.method)
}
inline std::string* RpcMessage::mutable_method() {
  _has_bits_[0] |= 0x00000002u;
  // @@protoc_insertion_point(field_mutable:netcore.RpcMessage.method)
  return method_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* RpcMessage::release_method() {
  // @@protoc_insertion_point(field_release:netcore.RpcMessage.method)
  if (!has_method()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000002u;
  return method_.ReleaseNonDefaultNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void RpcMessage::set_allocated_method(std::string* method) {
  if (method != nullptr) {
    _has_bits_[0] |= 0x00000002u;
  } else {
    _has_bits_[0] &= ~0x00000002u;
  }
  method_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), method);
  // @@protoc_insertion_point(field_set_allocated:netcore.RpcMessage.method)
}

// optional bytes message = 3;
inline bool RpcMessage::has_message() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void RpcMessage::clear_message() {
  message_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  _has_bits_[0] &= ~0x00000004u;
}
inline const std::string& RpcMessage::message() const {
  // @@protoc_insertion_point(field_get:netcore.RpcMessage.message)
  return message_.GetNoArena();
}
inline void RpcMessage::set_message(const std::string& value) {
  _has_bits_[0] |= 0x00000004u;
  message_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:netcore.RpcMessage.message)
}
inline void RpcMessage::set_message(std::string&& value) {
  _has_bits_[0] |= 0x00000004u;
  message_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:netcore.RpcMessage.message)
}
inline void RpcMessage::set_message(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  _has_bits_[0] |= 0x00000004u;
  message_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:netcore.RpcMessage.message)
}
inline void RpcMessage::set_message(const void* value, size_t size) {
  _has_bits_[0] |= 0x00000004u;
  message_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:netcore.RpcMessage.message)
}
inline std::string* RpcMessage::mutable_message() {
  _has_bits_[0] |= 0x00000004u;
  // @@protoc_insertion_point(field_mutable:netcore.RpcMessage.message)
  return message_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* RpcMessage::release_message() {
  // @@protoc_insertion_point(field_release:netcore.RpcMessage.message)
  if (!has_message()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000004u;
  return message_.ReleaseNonDefaultNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void RpcMessage::set_allocated_message(std::string* message) {
  if (message != nullptr) {
    _has_bits_[0] |= 0x00000004u;
  } else {
    _has_bits_[0] &= ~0x00000004u;
  }
  message_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), message);
  // @@protoc_insertion_point(field_set_allocated:netcore.RpcMessage.message)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace netcore

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::netcore::ErrorCode> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::netcore::ErrorCode>() {
  return ::netcore::ErrorCode_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_rpcmessage_2eproto