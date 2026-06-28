#!/usr/bin/env -S PYTHONPATH=../../../tools/extract-utils python3
#
# SPDX-FileCopyrightText: 2024 The LineageOS Project
# SPDX-License-Identifier: Apache-2.0
#

from extract_utils.fixups_blob import (
    BlobFixupCtx,
    File,
    blob_fixup,
    blob_fixups_user_type,
)
from extract_utils.fixups_lib import (
    lib_fixups,
    lib_fixups_user_type,
)
from extract_utils.main import (
    ExtractUtils,
    ExtractUtilsModule,
)
from extract_utils.tools import (
    llvm_objdump_path,
)
from extract_utils.utils import (
    run_cmd,
)

namespace_imports = [
    'device/oneplus/sm8250-common',
    'hardware/oplus',
    'hardware/qcom-caf/sm8250',
    'hardware/qcom-caf/wlan',
    'vendor/qcom/opensource/commonsys-intf/display',
    'vendor/qcom/opensource/commonsys/display',
    'vendor/qcom/opensource/dataservices',
    'vendor/qcom/opensource/display',
]


def lib_fixup_vendor_suffix(lib: str, partition: str, *args, **kwargs):
    return f'{lib}_vendor' if partition in ['odm', 'vendor'] else None


lib_fixups: lib_fixups_user_type = {
    **lib_fixups,
    (
        'com.qti.stats.pdlib',
        'com.qualcomm.qti.dpm.api@1.0',
        'libmmosal',
        'vendor.qti.hardware.wifidisplaysession@1.0',
        'vendor.qti.imsrtpservice@3.0',
    ): lib_fixup_vendor_suffix,
}


def blob_fixup_nop_call(
    ctx: BlobFixupCtx,
    file: File,
    file_path: str,
    call_instruction: str,
    disassemble_symbol: str,
    symbol: str,
    *args,
    **kwargs,
):
    for line in run_cmd(
        [
            llvm_objdump_path,
            f'--disassemble-symbols={disassemble_symbol}',
            file_path,
        ]
    ).splitlines():
        line = line.split(maxsplit=3)

        if len(line) != 4:
            continue

        offset, _, instruction, args = line

        if instruction != call_instruction:
            continue

        if not args.endswith(f' <{symbol}>'):
            continue

        with open(file_path, 'rb+') as f:
            f.seek(int(offset[:-1], 16))
            f.write(b'\x1f\x20\x03\xd5')  # AArch64 NOP

        break


blob_fixups: blob_fixups_user_type = {
    'odm/bin/hw/vendor.oplus.hardware.biometrics.fingerprint@2.1-service': blob_fixup()
        .add_needed('libshims_fingerprint.oplus.so'),
    'odm/etc/vintf/manifest/manifest_oplus_fingerprint.xml': blob_fixup()
        .patch_file('blob-patches/manifest_oplus_fingerprint.patch'),
    ('odm/lib64/mediadrm/libwvdrmengine.so', 'odm/lib64/libwvhidl.so'): blob_fixup()
        .add_needed('libcrypto_shim.so'),
    'odm/lib64/libpwirissoft.so': blob_fixup()
        .call(blob_fixup_nop_call, 'bl', '_ZN4pxlw18qcomSetDisplayModeEPvii', '_ZN4pxlw15irisSetConfigExEiiPKi@plt'),
    'product/app/PowerOffAlarm/PowerOffAlarm.apk': blob_fixup()
        .apktool_patch('blob-patches/PowerOffAlarm.patch'),
    'product/etc/sysconfig/com.android.hotwordenrollment.common.util.xml': blob_fixup()
        .regex_replace('/my_product', '/product'),
    'system_ext/lib64/libwfdnative.so': blob_fixup()
        .add_needed('libinput_shim.so'),
    'vendor/etc/libnfc-nci.conf': blob_fixup()
        .regex_replace('NFC_DEBUG_ENABLED=1', 'NFC_DEBUG_ENABLED=0'),
    'vendor/etc/libnfc-nxp.conf': blob_fixup()
        .regex_replace('(NXPLOG_.*_LOGLEVEL)=0x03', '\\1=0x02')
        .regex_replace('NFC_DEBUG_ENABLED=1', 'NFC_DEBUG_ENABLED=0'),
    'vendor/etc/msm_irqbalance.conf': blob_fixup()
        .regex_replace('IGNORED_IRQ=27,23,38$', 'IGNORED_IRQ=27,23,38,115,332'),
    'vendor/lib64/hw/com.qti.chi.override.so': blob_fixup()
        .add_needed('libcamera_metadata_shim.so')
        .binary_regex_replace(b'com.oem.autotest', b'\x00om.oem.autotest'),
    ('vendor/lib64/libarcsoft_hta.so', 'vendor/lib64/libarcsoft_superportrait.so', 'vendor/lib64/libarcsoft_hdrplus_hvx_stub.so', 'vendor/lib64/libarcsoft_high_dynamic_range_v4.so', 'vendor/lib64/libarcsoft_mfsr_frt.so', 'vendor/lib64/libarcsoft_super_night_raw.so', 'vendor/lib64/libarcsoft_dualcam_refocus_preview.so'): blob_fixup()
        .clear_symbol_version('remote_handle_close')
        .clear_symbol_version('remote_handle_invoke')
        .clear_symbol_version('remote_handle_open')
        .clear_symbol_version('remote_register_buf_attr')
        .clear_symbol_version('remote_register_buf'),
    'vendor/lib64/libdpps.so': blob_fixup()
        .replace_needed('libtinyxml2.so', 'libtinyxml2-v34.so'),
    'vendor/lib64/libmidasserviceintf_aidl.so': blob_fixup()
        .remove_needed('android.frameworks.stats@1.0.so'),
    'vendor/lib64/sensors.ssc.so': blob_fixup()
        .binary_regex_replace(b'qti.sensor.wise_light', b'android.sensor.light\x00')
        .sig_replace('F1 E9 D3 84 52 49 3F A0 72', 'F1 A9 00 80 52 09 00 A0 72'),
    'vendor/lib64/vendor.qti.hardware.camera.postproc@1.0-service-impl.so': blob_fixup()
        .call(blob_fixup_nop_call, 'bl', '__cfi_check', '_ZN7android8hardware22configureRpcThreadpoolEmb@plt'),
}  # fmt: skip

module = ExtractUtilsModule(
    'sm8250-common',
    'oneplus',
    blob_fixups=blob_fixups,
    lib_fixups=lib_fixups,
    namespace_imports=namespace_imports,
)

if __name__ == '__main__':
    utils = ExtractUtils.device(module)
    utils.run()
