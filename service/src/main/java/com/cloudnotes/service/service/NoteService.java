package com.cloudnotes.service.service;

import com.cloudnotes.service.common.Result;
import com.cloudnotes.service.entity.Note;
import com.cloudnotes.service.entity.TextBlock;

import java.util.List;

public interface NoteService {
    public List<Note> find(Integer cid);
    public Result create(Note note);
    public Result delete(Integer id);
    public Result update(Note note);
    public Result findText(Integer nid);
    public Result updateText(TextBlock textBlock);
    public Result updatePower(Note note);
    public Result shareNew(Note note);
}
