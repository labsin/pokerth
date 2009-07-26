/*
 * Generated by asn1c-0.9.22 (http://lionet.info/asn1c)
 * From ASN.1 module "POKERTH-PROTOCOL"
 * 	found in "../../../docs/pokerth.asn1"
 */

#include <asn_internal.h>

#include "AvatarReplyMessage.h"

static asn_TYPE_member_t asn_MBR_avatarResult_3[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct avatarResult, choice.avatarHeader),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_AvatarHeader,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"avatarHeader"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct avatarResult, choice.avatarData),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_AvatarData,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"avatarData"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct avatarResult, choice.avatarEnd),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_AvatarEnd,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"avatarEnd"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct avatarResult, choice.unknownAvatar),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UnknownAvatar,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"unknownAvatar"
		},
};
static asn_TYPE_tag2member_t asn_MAP_avatarResult_tag2el_3[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* avatarHeader at 113 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* avatarData at 114 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* avatarEnd at 115 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* unknownAvatar at 117 */
};
static asn_CHOICE_specifics_t asn_SPC_avatarResult_specs_3 = {
	sizeof(struct avatarResult),
	offsetof(struct avatarResult, _asn_ctx),
	offsetof(struct avatarResult, present),
	sizeof(((struct avatarResult *)0)->present),
	asn_MAP_avatarResult_tag2el_3,
	4,	/* Count of tags in the map */
	0,
	4	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_avatarResult_3 = {
	"avatarResult",
	"avatarResult",
	CHOICE_free,
	CHOICE_print,
	CHOICE_constraint,
	CHOICE_decode_ber,
	CHOICE_encode_der,
	CHOICE_decode_xer,
	CHOICE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	CHOICE_outmost_tag,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	0,	/* No PER visible constraints */
	asn_MBR_avatarResult_3,
	4,	/* Elements count */
	&asn_SPC_avatarResult_specs_3	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_AvatarReplyMessage_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct AvatarReplyMessage, requestId),
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_NonZeroId,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"requestId"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct AvatarReplyMessage, avatarResult),
		-1 /* Ambiguous tag (CHOICE?) */,
		0,
		&asn_DEF_avatarResult_3,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"avatarResult"
		},
};
static ber_tlv_tag_t asn_DEF_AvatarReplyMessage_tags_1[] = {
	(ASN_TAG_CLASS_APPLICATION | (3 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_TYPE_tag2member_t asn_MAP_AvatarReplyMessage_tag2el_1[] = {
    { (ASN_TAG_CLASS_UNIVERSAL | (2 << 2)), 0, 0, 0 }, /* requestId at 111 */
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 1, 0, 0 }, /* avatarHeader at 113 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* avatarData at 114 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 1, 0, 0 }, /* avatarEnd at 115 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 1, 0, 0 } /* unknownAvatar at 117 */
};
static asn_SEQUENCE_specifics_t asn_SPC_AvatarReplyMessage_specs_1 = {
	sizeof(struct AvatarReplyMessage),
	offsetof(struct AvatarReplyMessage, _asn_ctx),
	asn_MAP_AvatarReplyMessage_tag2el_1,
	5,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	1,	/* Start extensions */
	3	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_AvatarReplyMessage = {
	"AvatarReplyMessage",
	"AvatarReplyMessage",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_AvatarReplyMessage_tags_1,
	sizeof(asn_DEF_AvatarReplyMessage_tags_1)
		/sizeof(asn_DEF_AvatarReplyMessage_tags_1[0]) - 1, /* 1 */
	asn_DEF_AvatarReplyMessage_tags_1,	/* Same as above */
	sizeof(asn_DEF_AvatarReplyMessage_tags_1)
		/sizeof(asn_DEF_AvatarReplyMessage_tags_1[0]), /* 2 */
	0,	/* No PER visible constraints */
	asn_MBR_AvatarReplyMessage_1,
	2,	/* Elements count */
	&asn_SPC_AvatarReplyMessage_specs_1	/* Additional specs */
};

