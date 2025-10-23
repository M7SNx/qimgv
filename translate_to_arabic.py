#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Arabic Translation Helper for qimgv
This script translates the English strings in ar_AR.ts to Arabic
"""

import re

# Dictionary of English to Arabic translations
translations = {
    # ChangelogWindow
    "Close, never show changelogs": "إغلاق، عدم إظهار سجل التغييرات مرة أخرى",
    "Close": "إغلاق",
    
    # ContextMenu
    "Edit": "تحرير",
    "Print": "طباعة",
    "Quick copy": "نسخ سريع",
    "Quick move": "نقل سريع",
    "Move to trash": "نقل إلى سلة المهملات",
    "Open": "فتح",
    "Folder View": "عرض المجلد",
    "Settings": "الإعدادات",
    "Open with...": "فتح بواسطة...",
    "Show in folder": "إظهار في المجلد",
    "Back": "رجوع",
    "Configure menu": "تكوين القائمة",
    
    # CopyOverlay
    "(1 / 1)": "(١ / ١)",
    "Copy to...": "نسخ إلى...",
    "Move to...": "نقل إلى...",
    
    # Core
    "Updated: ": "تم التحديث: ",
    "Welcome to ": "مرحباً بك في ",
    " version ": " إصدار ",
    "Shuffle mode: OFF": "وضع العشوائي: متوقف",
    "Shuffle mode: ON": "وضع العشوائي: مفعل",
    "Slideshow: OFF": "عرض الشرائح: متوقف",
    "Slideshow: ON": "عرض الشرائح: مفعل",
    "Delete ": "حذف ",
    " items permanently?": " عناصر نهائياً؟",
    "Delete item permanently?": "حذف العنصر نهائياً؟",
    "Delete permanently": "حذف نهائياً",
    "File removed": "تم حذف الملف",
    "Removed: ": "تم الحذف: ",
    " files": " ملفات",
    "Move ": "نقل ",
    " items to trash?": " عناصر إلى سلة المهملات؟",
    "Move item to trash?": "نقل العنصر إلى سلة المهملات؟",
    "Moved to trash": "تم النقل إلى سلة المهملات",
    "Moved to trash: ": "تم النقل إلى سلة المهملات: ",
    "File copied": "تم نسخ الملف",
    "Copied: ": "تم النسخ: ",
    "File moved": "تم نقل الملف",
    "Moved: ": "تم النقل: ",
    "Could not remove location from file list": "لا يمكن إزالة الموقع من قائمة الملفات",
    "Can not rename - destination exists or invalid": "لا يمكن إعادة التسمية - الوجهة موجودة أو غير صالحة",
    "Error: unsupported file format": "خطأ: تنسيق ملف غير مدعوم",
    "Error: could not read file": "خطأ: تعذر قراءة الملف",
    "Error: unable to read - file does not exist.": "خطأ: تعذرت القراءة - الملف غير موجود.",
    "Invalid file": "ملف غير صالح",
    "End of directory.": "نهاية المجلد.",
    "No file opened.": "لم يتم فتح أي ملف.",
    "Error: directory does not exist.": "خطأ: المجلد غير موجود.",
    "Can't remove. File does not exist.": "لا يمكن الحذف. الملف غير موجود.",
    "Error: cannot create directory.": "خطأ: لا يمكن إنشاء المجلد.",
    "Error: could not create file list.": "خطأ: تعذر إنشاء قائمة الملفات.",
    
    # CropPanel
    "Original": "الأصلي",
    "Confirm": "تأكيد",
    "Accept (Enter)": "قبول (Enter)",
    "Discard": "تجاهل",
    "Cancel (Esc)": "إلغاء (Esc)",
    
    # FileReplaceDialog
    "Yes": "نعم",
    "No": "لا",
    "Skip all": "تخطي الكل",
    "Cancel": "إلغاء",
    "File exists": "الملف موجود",
    "Confirm replace": "تأكيد الاستبدال",
    "Replace": "استبدال",
    "Replace all": "استبدال الكل",
    "Replacing:": "الاستبدال:",
    "With:": "بـ:",
    
    # FolderView
    "Folders": "المجلدات",
    "Back (Backspace)": "رجوع (Backspace)",
    "Up": "أعلى",
    
    # ImageInfoOverlay
    "Image info": "معلومات الصورة",
    "File name:": "اسم الملف:",
    "Directory:": "المجلد:",
    "File size:": "حجم الملف:",
    "Creation date:": "تاريخ الإنشاء:",
    "Modification date:": "تاريخ التعديل:",
    "Image format:": "تنسيق الصورة:",
    "Resolution:": "الدقة:",
    " (VA)": " (VA)",
    
    # KeySequenceEdit
    "Press shortcut": "اضغط الاختصار",
    "Catch shortcut": "التقاط الاختصار",
    "Clear": "مسح",
    
    # MainWindow
    "Sort by name": "ترتيب حسب الاسم",
    "Sort by newest": "ترتيب حسب الأحدث",
    "Sort by size": "ترتيب حسب الحجم",
    "Ascending": "تصاعدي",
    "Descending": "تنازلي",
    "Fit Window": "ملائمة النافذة",
    "Fit Width": "ملائمة العرض",
    "Fit 1:1": "ملائمة 1:1",
    "Error initializing mpv library": "خطأ في تهيئة مكتبة mpv",
    "Could not create thumbnails": "تعذر إنشاء المصغرات",
    "Shortcuts list": "قائمة الاختصارات",
    
    # PathSelectorMenuItem
    "Add folder": "إضافة مجلد",
    "Last used": "آخر استخدام",
    "Favorites": "المفضلة",
    "Bookmarks": "العلامات المرجعية",
    
    # PrintDialog
    "Select printer": "اختر الطابعة",
    
    # RenameOverlay
    "Rename:": "إعادة تسمية:",
    "Press Enter to accept, Esc to cancel": "اضغط Enter للقبول، Esc للإلغاء",
    "New name:": "الاسم الجديد:",
    "Rename": "إعادة تسمية",
    "File renamed": "تم إعادة تسمية الملف",
    "Error: file name empty": "خطأ: اسم الملف فارغ",
    
    # ResizeDialog
    "Resize": "تغيير الحجم",
    "Resize width:": "تغيير العرض:",
    "Resize height:": "تغيير الارتفاع:",
    "Keep aspect ratio": "الحفاظ على نسبة العرض إلى الارتفاع",
    "Filter:": "التصفية:",
    "Quality:": "الجودة:",
    
    # SaveConfirmOverlay
    "Unsaved edits": "تعديلات غير محفوظة",
    "Save": "حفظ",
    "Save as": "حفظ باسم",
    "Don't save": "عدم الحفظ",
    
    # Script
    "Script: \"": "البرنامج النصي: \"",
    "\" is not found": "\" غير موجود",
    "Script output: ": "مخرجات البرنامج النصي: ",
    "Running script: ": "تشغيل البرنامج النصي: ",
    "Script finished: ": "انتهى البرنامج النصي: ",
    
    # ScriptEditorDialog
    "Script name: ": "اسم البرنامج النصي: ",
    "Command: ": "الأمر: ",
    "Arguments: ": "الوسائط: ",
    "Options: ": "الخيارات: ",
    "Blocking": "حظر",
    "Wait script to finish before displaying next image": "انتظر انتهاء البرنامج النصي قبل عرض الصورة التالية",
    "Filter": "تصفية",
    "Script modifies current image": "البرنامج النصي يعدل الصورة الحالية",
    "Save": "حفظ",
    "Discard": "تجاهل",
    "Save shortcut": "حفظ الاختصار",
    "Discard shortcut": "تجاهل الاختصار",
    
    # SettingsDialog
    "View": "عرض",
    "Controls": "التحكم",
    "Scripts": "البرامج النصية",
    "Advanced": "متقدم",
    "Interface": "الواجهة",
    "Scaling": "التحجيم",
    "Theme": "السمة",
    "Background": "الخلفية",
    "Accent": "التمييز",
    "Slideshow": "عرض الشرائح",
    "Video": "الفيديو",
    "Folders": "المجلدات",
    "Window": "النافذة",
    "General settings": "الإعدادات العامة",
    "Show overlay": "إظهار التراكب",
    "Centered": "في الوسط",
    "Simple zoom": "تكبير بسيط",
    "Smooth animation": "حركة سلسة",
    "Always on top": "دائماً في الأعلى",
    "Use checkerboard grid": "استخدام شبكة رقعة الشطرنج",
    "Expand images": "توسيع الصور",
    "Show info bar": "إظهار شريط المعلومات",
    "Show image name": "إظهار اسم الصورة",
    "Mouse": "الماوس",
    "Keyboard": "لوحة المفاتيح",
    "Script": "برنامج نصي",
    "Add script": "إضافة برنامج نصي",
    "Edit": "تحرير",
    "Remove": "إزالة",
    "Maximum image size": "الحد الأقصى لحجم الصورة",
    "Apply": "تطبيق",
    "Default": "افتراضي",
    "OK": "موافق",
    "Confirm exit": "تأكيد الخروج",
    "Exit fullscreen on close": "الخروج من ملء الشاشة عند الإغلاق",
    "Use KDE blur": "استخدام ضبابية KDE",
    "Use high quality scaling": "استخدام تحجيم عالي الجودة",
    "Fit mode": "وضع الملائمة",
    "Instant": "فوري",
    "Zoom step": "خطوة التكبير",
    "Scaling filter": "مرشح التحجيم",
    "Disable smooth upscaling": "تعطيل التكبير السلس",
    "Disable smooth downscaling": "تعطيل التصغير السلس",
    "Remember zoom level": "تذكر مستوى التكبير",
    "Remember pan position": "تذكر موضع الانتقال",
    "Transparency grid": "شبكة الشفافية",
    "Language": "اللغة",
    "Show directory on startup": "إظهار المجلد عند بدء التشغيل",
    "Image info overlay": "تراكب معلومات الصورة",
    "Show folder on startup": "إظهار المجلد عند بدء التشغيل",
    "Panel position": "موضع اللوحة",
    "Thumbnail size": "حجم المصغرة",
    "Loop through images": "التكرار عبر الصور",
    "Rotation speed": "سرعة التدوير",
    "Default timer": "المؤقت الافتراضي",
    "Show full video controls on mouse hover": "إظهار عناصر تحكم الفيديو الكاملة عند تمرير الماوس",
    "Seek through video frames": "البحث عبر إطارات الفيديو",
    "Pause video on playback": "إيقاف الفيديو مؤقتاً عند التشغيل",
    "Volume": "الصوت",
    "Mute": "كتم الصوت",
    "Quick filter": "تصفية سريعة",
    "Supported images": "الصور المدعومة",
    "Videos": "الفيديوهات",
    "None": "لا شيء",
    "Active": "نشط",
    "Show hidden files": "إظهار الملفات المخفية",
    "Image list mode": "وضع قائمة الصور",
    "Create thumbnails": "إنشاء مصغرات",
    "Current dir": "المجلد الحالي",
    "Current dir + subdirs": "المجلد الحالي + المجلدات الفرعية",
    "All files": "جميع الملفات",
    "Window mode": "وضع النافذة",
    "Fullscreen": "ملء الشاشة",
    "Borderless": "بدون حدود",
    "Remember window geometry": "تذكر هندسة النافذة",
    "Windowed (normal)": "نافذة (عادي)",
    "Mouse cursor timeout (ms)": "مهلة مؤشر الماوس (ميلي ثانية)",
    "Cursor autohide": "إخفاء المؤشر تلقائياً",
    "Enable context menu": "تمكين قائمة السياق",
    
    # Shortcuts
    "Exit fullscreen": "الخروج من ملء الشاشة",
    "Toggle fullscreen": "تبديل ملء الشاشة",
    "Crop": "قص",
    "Crop to selection": "قص إلى التحديد",
    "Flip H": "قلب أفقي",
    "Flip V": "قلب عمودي",
    "Rotate left": "تدوير لليسار",
    "Rotate right": "تدوير لليمين",
    "Quick save": "حفظ سريع",
    "Zoom in": "تكبير",
    "Zoom out": "تصغير",
    "Toggle fit mode": "تبديل وضع الملائمة",
    "Jump to first": "الانتقال إلى الأول",
    "Jump to last": "الانتقال إلى الأخير",
    "Open URL": "فتح رابط URL",
    "Fit window": "ملائمة النافذة",
    "Fit 1:1": "ملائمة 1:1",
    "Fit width": "ملائمة العرض",
    "Next image": "الصورة التالية",
    "Previous image": "الصورة السابقة",
    "Zoom toggle": "تبديل التكبير",
    "Shuffle": "عشوائي",
    "Start slideshow": "بدء عرض الشرائح",
    "Stop slideshow": "إيقاف عرض الشرائح",
    "Pause slideshow": "إيقاف عرض الشرائح مؤقتاً",
    "Resume slideshow": "استئناف عرض الشرائح",
    "Play video": "تشغيل الفيديو",
    "Pause video": "إيقاف الفيديو مؤقتاً",
    "Video seek forward": "البحث للأمام في الفيديو",
    "Video seek backward": "البحث للخلف في الفيديو",
    "Frame step": "خطوة الإطار",
    "Frame step back": "خطوة الإطار للخلف",
    "Seek to frame": "البحث إلى إطار",
    "Volume up": "رفع الصوت",
    "Volume down": "خفض الصوت",
    "Toggle mute": "تبديل كتم الصوت",
    "Toggle transparency grid": "تبديل شبكة الشفافية",
    "Toggle image info": "تبديل معلومات الصورة",
    "Show info": "إظهار المعلومات",
    
    # VideoControlsProxyWrapper
    "Volume: ": "الصوت: ",
    
    # ViewerWidget
    " Zoom: ": " التكبير: ",
    "FIT": "ملائمة",
    
    # Common terms
    "Left": "يسار",
    "Right": "يمين",
    "Top": "أعلى",
    "Bottom": "أسفل",
    "Start": "بدء",
    "Stop": "إيقاف",
    "Pause": "إيقاف مؤقت",
    "Play": "تشغيل",
    "Loading...": "جار التحميل...",
    "Error": "خطأ",
    "Warning": "تحذير",
    "Information": "معلومات",
    "About": "حول",
    "Help": "مساعدة",
    "Version": "الإصدار",
    "Author": "المؤلف",
    "License": "الترخيص",
}

def translate_file(input_file, output_file):
    """Read the .ts file and replace translation strings"""
    with open(input_file, 'r', encoding='utf-8') as f:
        content = f.read()
    
    # Replace each translation
    for english, arabic in translations.items():
        # Escape special regex characters
        escaped_english = re.escape(english)
        # Find pattern: <source>English text</source>\n        <translation type="unfinished"></translation>
        pattern = f'(<source>{escaped_english}</source>\\s*<translation type="unfinished">)</translation>'
        replacement = f'\\1{arabic}</translation>'
        content = re.sub(pattern, replacement, content)
    
    # Write the translated content
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write(content)
    
    print(f"Translation completed! Check {output_file}")
    print(f"Translated {len(translations)} strings.")

if __name__ == "__main__":
    input_file = r"C:\Users\m7\Desktop\Projects\Image Viewer - qimgv\qimgv\qimgv\res\translations\ar_AR.ts"
    output_file = r"C:\Users\m7\Desktop\Projects\Image Viewer - qimgv\qimgv\qimgv\res\translations\ar_AR_translated.ts"
    
    translate_file(input_file, output_file)
